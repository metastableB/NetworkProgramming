/*
 * @author:metastableB
 * handler.cpp
 */

 #include "handler.h"
 
 #ifdef _DEBUG_
 std::chrono::high_resolution_clock::time_point epoch_start;
 long long int print_time_(std::chrono::high_resolution_clock::time_point t){
	return (t - epoch_start).count();
}

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

Handler::Handler(std::chrono::duration<long,std::micro> d, std::vector<Source> *s_l, Switch *s){
	simulation_duration = d;
	source_list = s_l;
	sw = s;
	if(simulation_duration <= std::chrono::nanoseconds(0))
		throw std::invalid_argument("Duration for simulation is invalid\n");
	if(no_sources <= 0)
		throw std::invalid_argument("Number of sources not set!\n");
}

void Handler::simulate(){
	std::cout << "Initializing initial environment....\n";
	initialize();
	std::cout << "Finished initializing\n";
	std::cout << "Simulation duration of " << simulation_duration.count() << " micro seconds\n";
	std::cout << "Now passing control to event loop.\n";
	auto start = std::chrono::high_resolution_clock::now();
	while(std::chrono::high_resolution_clock::now() - start <  simulation_duration){
		auto t = event_queue.top();
		event_queue.pop();
		transition(t);
		auto intr = std::chrono::high_resolution_clock::now();
		// Printing every 10 second interval
		if(((intr -  start).count() % 10000000) == 0)
			std::cout << "Time remaining: " << (((simulation_duration -(intr-start)).count())/1000000000)
				<< " seconds\n";
	}
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "Simulation finished successfully\n";
	std::cout << "Run time " << (stop - start).count()/1000000000 << " seconds \n";
}
void Handler::initialize(){
	/* 
	 * At this point we have all the initialized sources and switch. 
	 * we just need to start the simulations and log the output
	 */
#ifdef _DEBUG_
	 epoch_start = std::chrono::high_resolution_clock::now();
	 epoch = epoch_start;
#else
	 epoch = std::chrono::high_resolution_clock::now();
#endif

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
	Packet* p = src->generate_packet();
#ifdef _DEBUG_
	auto old_tp = temporal->key;
#endif
	if(p->p_state == Packet::packet_state::LOST_SRC)
		//TODO Handle lost packets;
		;
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
	sw->arrival_handler(temporal->packet);
#if _DEBUG_
	printf("SWITCH queued packet %d from %d.\n",temporal->packet->get_id(),
		temporal->source->get_id());
#endif
	delete temporal->packet;
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
