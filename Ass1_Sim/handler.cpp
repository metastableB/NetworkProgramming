/*
 * @author:metastableB
 * handler.cpp
 */

 #include "handler.h"
 
std::chrono::high_resolution_clock::time_point epoch_start;
long long int print_time_(std::chrono::high_resolution_clock::time_point t){
	return (t - epoch_start).count();
}
 #ifdef _DEBUG_
void print_event_(Temporal_Object* temporal){
	printf("%d %s at %lld\n",temporal->source->get_id(),
		temporal->get_string_state().c_str(),print_time_(temporal->key));
}
#endif

#ifdef _BREAK_POINTS_
void break_point_(){
	char a;
	std::cin >> a;
}
#endif	

Handler::Handler(std::chrono::duration<long,std::micro> d, std::vector<Source> *s_l, Switch *s,bool in_handler_write){
	simulation_duration = d;
	source_list = s_l;
	sw = s;
	if(simulation_duration <= std::chrono::nanoseconds(0))
		throw std::invalid_argument("Duration for simulation is invalid\n");
	if(no_sources <= 0)
		throw std::invalid_argument("Number of sources not set!\n");
	if(in_handler_write)
		fp = fopen(OUTPUT_FILE,"w");
	/* variables for event logging */
	num_packets_lost_src = 0;
	num_packets_queued_src = 0;
	num_packets_lost_sw = 0;
	average_queuing_delay = std::chrono::microseconds(0);
	average_sw_queue_size = 0;
	num_packets_received_sw = 0;
	average_source_queue_size = 0;
	num_packets_transmitted = 0;
}

void Handler::set_fp(FILE* f){
	fp = f;
}

void Handler::simulate(){
#ifdef SIM_PRINT
	std::cout << "\nSimulation duration of " << simulation_duration.count() << " micro seconds\n";
#endif
	initialize();
	auto intr = epoch;
	while(true){
		auto t = event_queue.top();
		event_queue.pop();
		intr = t->key;
		if(intr - epoch >  simulation_duration) break;
		transition(t);
		// Printing every 10 second interval
		if(((intr -  epoch).count() % 10000000) == 0)
			std::cout << "Time remaining: " << (((simulation_duration -(intr-epoch)).count())/1000000000)
				<< " seconds" <<std::endl;
	}
#ifdef SIM_PRINT
	std::cout << "Simulation finished successfully\n";
	std::cout << "Simulation time " << (intr - epoch).count()/1000000000 << " seconds \n";
#endif
}
void Handler::initialize(){
	/* 
	 * At this point we have all the initialized sources and switch. 
	 * we just need to start the simulations and log the output
	 */
	 epoch_start = std::chrono::high_resolution_clock::now();
	 epoch = epoch_start;
	 for(size_t i = 0; i < source_list->size(); i++){
	 	Source &x = source_list->at(i);
	 	if(x.get_max_queue_size() <= 0){
	 		std::string s = "Zero length queue not allowed! Source id " + std::to_string(x.get_id()) + "\n";
	 		throw std::logic_error(s);
	 	}

	 	Temporal_Object *t = new Temporal_Object(&x, 
	 		std::chrono::high_resolution_clock::now(),
	 		Temporal_Object::temporal_state::GENERATE);
	 	
	 	// Schedule a packet transmit about 5 nano seconds after creation
	 	Temporal_Object *t2 = new Temporal_Object(&x,
	 		t->key + std::chrono::microseconds(5),
	 		Temporal_Object::temporal_state::TRANSMIT);

	 	// Schedule a packet service about 10 nano seconds after creation
	 	Temporal_Object *t3 = new Temporal_Object(&x,
	 		t->key + std::chrono::microseconds(10),
	 		Temporal_Object::temporal_state::SERVICING);

	 	event_queue.push(t);
	 	event_queue.push(t2);
	 	event_queue.push(t3);
#ifdef _DEBUG_
	 	printf("GENERATE id %d time: %lld\n",x.get_id(),print_time_(t->key));
	 	printf("TRANSMIT id %d time: %lld\n",x.get_id(),print_time_(t2->key));
#endif
#ifdef _BREAK_POINTS_
	 	break_point_();
#endif
	 }
}

void Handler::transition(Temporal_Object* temporal){
#ifdef _DEBUG_
	print_event_(temporal);
#endif

 	switch (temporal->state) {
 		case Temporal_Object::temporal_state::GENERATE:
 			handler_generate(temporal);
 			break;
 		case Temporal_Object::temporal_state::TRANSMIT:
 			handler_transmit(temporal);
 			break;
 		case Temporal_Object::temporal_state::RECEIVED_SWITCH:
 			handler_received_switch(temporal);
 			break;
 		case Temporal_Object::temporal_state::SERVICING:
 			handler_servicing(temporal);
 			break;
 		default:
 			throw std::logic_error("Unhandled Temporal_Object state");
 	}

#ifdef _BREAK_POINTS_
	 	break_point_();
#endif
 } 

Handler::~Handler(){
	while(!event_queue.empty()){
		auto x = event_queue.top();
		event_queue.pop();
		if(x->packet != nullptr)
			delete x->packet;
		delete x;
	}
}	
void Handler::handler_generate(Temporal_Object* temporal){
	// This method independently creates packets at the sending rate
	// These packets are queued at SRC and the QUEUED_SRC state becomes
	// redundant. 
	Source* src = temporal->source;
	Packet* p = src->generate_packet(temporal->key);
#ifdef _DEBUG_
	auto old_tp = temporal->key;
#endif
	if(p->p_state == Packet::packet_state::LOST_SRC){
		if(p->get_source_id() == 0)
			num_packets_lost_src++;
		delete p;
	} else {
		p->queue_time_point = temporal->key;
#ifdef OUT_QUEUE_VS_TIME
		if(p->get_source_id() == 0)
			fprintf(fp,"%ld\t%ld\n",(temporal->key-epoch).count()/10000000,temporal->source->get_queue_size());
#endif
	}
	// Schedule next generation time for this source.
	temporal->key = src->get_next_sending_time_point(temporal->key)+std::chrono::milliseconds(1);
	event_queue.push(temporal);
#ifdef _DEBUG_
	printf("%d generated packet at %lld\n",src->get_id(),print_time_(old_tp));
#endif
}
	
void Handler::handler_queued_src(Temporal_Object* temporal) {
	throw std::runtime_error("This should not happen! A hadle_queued_stc call ?");
	return;
}

void Handler::handler_transmit(Temporal_Object* temporal) {
	// This is the transmission event for the source that works independently
	// of the generation event.
	Source* src = temporal->source;
	Packet* p = src->get_packet_for_dispatch();

	if(p != nullptr){
#ifdef _DEBUG_
		std::cout << src->get_id() << " transfered packet: " << p->get_id() << "\n";
#endif
		// Calculating average queuing delay
		if(p->get_source_id() == 0){
			auto qtp = p->get_queueing_delay(temporal->key);
			average_queuing_delay = (average_queuing_delay*num_packets_queued_src + qtp);
			average_queuing_delay = std::chrono::nanoseconds(average_queuing_delay/(++num_packets_queued_src));
			//std::cout << "a q d "<< average_queuing_delay.count() << std::endl;
			//std::cout << "q d "<< qtp.count() << std::endl;
			//std::cout << "num packets\n" << (num_packets_queued_src) << std::endl;

		}
		temporal->packet = p;
		auto old_tp = temporal->key;
		temporal->key = src->get_next_arrival_time_point(old_tp);
		// when this event is dequeued again, it would have been received at switch
		temporal->state = Temporal_Object::temporal_state::RECEIVED_SWITCH;
		event_queue.push(temporal);	
		// schedule next transmission event
		Temporal_Object* t = new Temporal_Object(src,
			src->get_next_dispatching_time_point(old_tp),
	 		Temporal_Object::temporal_state::TRANSMIT);

		event_queue.push(t);
#ifdef _DEBUG_
		printf("%d sent packet at %lld. Next scheduled %lld.\n",src->get_id(),print_time_(old_tp),print_time_(temporal->key));
#endif
		
	} else {
		// The queue is empty. Ping after 5 ns
		temporal->key = temporal->key + src->get_dispatching_time_delta()/5;
		event_queue.push(temporal);
#ifdef _DEBUG_
		printf("%d found empty queue. Next scheduled %lld.\n",src->get_id(),print_time_(temporal->key));
#endif
	}
}

void Handler::handler_received_switch(Temporal_Object* temporal) {
	// enqueue the packet to the appropriate queue
	// delete this temporal object but not the packet
	auto  p = sw->arrival_handler(temporal->packet);
	if(p->p_state == Packet::packet_state::LOST_SW){
		num_packets_lost_sw++;
		delete p;
	}
	else{
		if(sw->switch_op_mode == Switch::switch_operating_mode::PACKET_SWITCHING){
			;
#ifdef SWITCH_QUEUE_VS_TIME
		if(p->get_source_id() == 0)
			fprintf(fp,"%ld\t%ld\n",(temporal->key-epoch).count()/10000000,sw->get_0_queue_size());
#endif
		}else if(p->get_source_id() == 0){
			;
		}

#if _DEBUG_
	printf("SWITCH queued packet %d from %d.\n",temporal->packet->get_id(),
		temporal->source->get_id());
#endif
	}
	delete temporal;
}

void Handler::handler_servicing(Temporal_Object* temporal) {
	// Pick up a packet and service it
	// Schedule the next servicing event
	// only one temporal object is required
	auto p = sw->service_packet();
	if(p!=nullptr) {
		temporal->key = sw->get_next_servicing_time_point(temporal->key);
		delete p;
#ifdef _DEBUG_
		printf("SWITCH serviced packet %d from %d. Next scheduled %lld\n",
		p->get_id(),p->get_source_id(),print_time_(temporal->key));
#endif
		
	} else {
		temporal->key = sw->get_next_servicing_time_point(temporal->key);

#ifdef _DEBUG_
		printf("SWITCH queue empty. Next scheduled %lld\n",print_time_(temporal->key));
#endif	
	}
	event_queue.push(temporal);
}