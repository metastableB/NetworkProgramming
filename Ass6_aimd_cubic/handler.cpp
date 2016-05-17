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

	packet_drop_prob = PACKET_DROP_PROBABILITY;
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

	 	event_queue.push(t);
	 	event_queue.push(t2);
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
	temporal->packet = nullptr;
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

	// This means we are transmitting a packet from source to switch
	if(temporal->packet == nullptr) {
		Source* src = temporal->source;
		Packet* p = src->get_packet_for_dispatch();

		if(p != nullptr){
//#ifdef _DEBUG_
			std::cout  << "send\t" << src->get_id() << "\t" << p->get_seq_num()<<"\t"<< p->get_id() <<"\n";
//#endif
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
			// or we are waiting for more ACKs
			temporal->key = temporal->key + src->get_dispatching_time_delta()/5;
			event_queue.push(temporal);
			if(temporal->packet != nullptr) throw std::logic_error("some logic error you are going to have a nightmare!");
#ifdef _DEBUG_
			printf("%d found empty queue. Next scheduled %lld.\n",src->get_id(),print_time_(temporal->key));
#endif
		}
	} else {
		if(!temporal->packet->is_packet_type(Packet::packet_type::ACK))
			throw std::logic_error("A non ack temporal message found. This should not happen");
		// we received an ack. we instruct the source to process the ack.
		Source* src = temporal->source;
		Packet* p = temporal->packet;
		std::cout << "ACK \t" << src->get_source_id() << "\t" << p->get_seq_num()<<"\t"<< p->get_id() << std::endl;
		// if retransmission is >=0 we resend that packet
		int retransmission = src->handle_ack(p);
		if( retransmission > 0){
			temporal->packet = src->get_packet_for_retransmission(retransmission);
			temporal->key = temporal->key + src->get_dispatching_time_delta()/8;
			// when this event is dequeued again, it would have been received at switch
			temporal->state = Temporal_Object::temporal_state::RECEIVED_SWITCH;
			event_queue.push(temporal);	
		} else delete temporal;
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
		Source *x = get_source_by_id(p->get_source_id());
	 	// We got a packet at switch. Schedule a servicing event
	 	Temporal_Object *t3 = new Temporal_Object(x,
	 		sw->get_next_servicing_time_point(temporal->key),
 		Temporal_Object::temporal_state::SERVICING);
 		event_queue.push(t3);
	}
	delete temporal;
}

void Handler::handler_servicing(Temporal_Object* temporal) {
	// Pick up a packet and service it
	// Schedule the next servicing event
	// only one temporal object is required

	/* I did not have a sink object for assignment 1. I'm just hacking around now.
	 * Too lazy to implement it now at 01:52am in the morning.
	 */

	auto p = sw->service_packet();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    double r = dis(gen);
    if(r < packet_drop_prob) {delete p; p = nullptr;}

	if(p!=nullptr) {
#ifdef _DEBUG_
		printf("SWITCH serviced packet %d from %d. Next scheduled %lld\n",
		p->get_id(),p->get_source_id(),print_time_(temporal->key));
#endif
		int src_id = p->get_source_id();
		Source* src = get_source_by_id(src_id);
		if(src == nullptr) throw std::logic_error("SOURCE NOT FOUND IN LIST! MY GOD!");
		std::cout << "r" << src_id <<"\t" << src->get_source_id() << "\t" << p->get_seq_num()<<"\t"<< p->get_id() << std::endl;
		int curr_window = src->get_curr_window();
		packet_list[src_id][p->get_seq_num()] = 1;
		int next_ack = get_next_ack(src_id, curr_window);
		std::cout << "send ack " << next_ack << std::endl;
		//delete p;
		if(next_ack > 0){
			if(next_ack == curr_window) reset_packet_list(src_id);
			auto old_tp = temporal->key;
			Temporal_Object* t = new Temporal_Object(src,
				src->get_next_dispatching_time_point(old_tp),
		 		Temporal_Object::temporal_state::TRANSMIT);
			Packet* p = src->generate_packet(temporal->key, Packet::packet_type::ACK);
			t->packet = p;
			p->set_seq_num(next_ack);
			event_queue.push(t);
		}
	} else {
		//This could happen if the packet was lost
	}
	delete temporal;
}

Source* Handler::get_source_by_id(int src_id){
	for(auto it = source_list->begin(); it != source_list->end(); it++)
		if((*it).get_source_id() == src_id) return &*it;
	return nullptr;
}

int Handler::get_next_ack(int src_id, int cw){
	auto &mp = packet_list[src_id];
	int i;
	bool received = false;
	for(i = 0; i < cw; i++)
		if(mp[i] == 1) {received = true;break;}
	for(i = 0; i < cw; i++)
		if(mp[i] != 1) break;
	if(i == 0 && received == false) return -1;
	return i;
}

void Handler::reset_packet_list(int src_id){
	packet_list[src_id].clear();
}