/*
 * @author:metastableB
 * handler.cpp
 */

 #include "handler.h"
 /* DEBUG */
 long long int print_time(std::chrono::system_clock::time_point t){
	time_t tt = std::chrono::system_clock::to_time_t(t);
	return tt;
}
/* debug */
Handler::Handler(std::chrono::duration<long,std::nano> d, std::vector<Source> *s_l, Switch *s){
	simulation_duration = d;
	source_list = s_l;
	sw = s;
	if(simulation_duration <= std::chrono::nanoseconds(0))
		throw std::invalid_argument("Duration for simulation is invalid\n");
	if(no_sources <= 0)
		throw std::invalid_argument("Number of sources not set!\n");
}

void Handler::simulate(){
	initialize();
	auto start = std::chrono::system_clock::now();
	while(std::chrono::system_clock::now() - start <  simulation_duration){
		auto t = event_queue.top();
		event_queue.pop();
		transition(t);
	}
}
void Handler::initialize(){
	/* 
	 * At this point we have all the initialized sources and switch. 
	 * we just need to start the simulations and log the output
	 */
	 for(size_t i = 0; i < source_list->size(); i++){
	 	Source &x = source_list->at(i);
	 	auto p = x.generate_packet();
	 	if(p == nullptr)
	 		throw std::runtime_error("Packet Initialization Error. `nullptr` returned\n");
	 	if(p->p_state == Packet::packet_state::LOST_SRC)
	 		throw std::runtime_error("First Packet lost. Is the source queue null?\n");
	 	
	 	// Schedule a packet creation for the source
	 	Temporal_Object *t = new Temporal_Object(&x, 
	 		std::chrono::system_clock::now(),
	 		Temporal_Object::temporal_state::GENERATE);
	 	
	 	// Schedule a packet transmit about 5 nano seconds after creation
	 	Temporal_Object *t2 = new Temporal_Object(&x,
	 		std::chrono::system_clock::now() + std::chrono::microseconds(5),
	 		Temporal_Object::temporal_state::TRANSMIT);

	 	event_queue.push(t);
	 	event_queue.push(t2);
	 	std::cout << "Done initialization\n";
	 	printf("GENERATE id %d time: %lld\n",x.get_id(),print_time(t->key));
	 	printf("TRANSMIT id %d time: %lld\n",x.get_id(),print_time(t2->key));
	 	char c;
	 	std::cin >> c;
	 }
}

void Handler::transition(Temporal_Object* temporal){
	printf("event received is ");
	time_t tt;
 	switch (temporal->state) {
 		case Temporal_Object::temporal_state::GENERATE:
 			handler_generate(temporal);
 			tt = std::chrono::system_clock::to_time_t(temporal->key);
 			printf("GENERATE at %d \n",tt);
 			break;
 		case Temporal_Object::temporal_state::TRANSMIT:
 			handler_transmit(temporal);
 			tt = std::chrono::system_clock::to_time_t(temporal->key);
 			printf("TRANSMIT at %d \n",tt);
 			break;
 		case Temporal_Object::temporal_state::RECEIVED_SWITCH:
 			handler_received_switch(temporal);
 			tt = std::chrono::system_clock::to_time_t(temporal->key);
 			printf("RECEIVED_SWITCH at %d \n",tt);
 			break;
 		case Temporal_Object::temporal_state::SERVICING:
 			handler_servicing(temporal);
 			tt = std::chrono::system_clock::to_time_t(temporal->key);
 			printf("SERVICING at %d \n",tt);
 			break;
 		default:
 			throw std::logic_error("Unhandled Temporal_Object state");
 	}
 	char c;
 	std::cin >> c;
 } 

Handler::~Handler(){
	while(!event_queue.empty()){
		auto x = event_queue.top();
		event_queue.pop();
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
	if(p->p_state == Packet::packet_state::LOST_SRC)
		//TODO Handle lost packets;
		;
	// Schedule next generation time for this source.
	temporal->key = src->get_next_sending_time_point(temporal->key)+std::chrono::milliseconds(1);
	event_queue.push(temporal);
	printf("Source %d generated a packet and scheduled next %d \n",src->get_id(),std::chrono::system_clock::to_time_t(temporal->key));
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
		printf("A packet sent from %d. Next scheduled.\n",src->get_id());
	} else {
		// The queue is empty. Ping after 5 ns
		temporal->key = temporal->key + src->get_dispatching_time_delta()/10;
		event_queue.push(temporal);
		printf("An enmpty queue for %d. Ping after %d ns\n",src->get_id(),std::chrono::system_clock::to_time_t(temporal->key));
	}
}

void Handler::handler_received_switch(Temporal_Object* temporal) {
	// received an object. we enqueue it according to switch properties and log the event
	delete temporal->packet;
	delete temporal;
	return;
}

void Handler::handler_servicing(Temporal_Object* temporal) {
	return;
}
