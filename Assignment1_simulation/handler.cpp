/*
 * @author:metastableB
 * handler.cpp
 */

 #include "handler.h"

 void Handler::transition(){
 	auto temporal = event_queue.top();
 	event_queue.pop();
 	switch (temporal->state) {
 		case Temporal_Object::temporal_state::GENERATED:
 			handler_generated(temporal);
 			break;
 		case Temporal_Object::temporal_state::QUEUED_AT_SRC:
 			handler_queued_at_src(temporal);
 			break;
 		case Temporal_Object::temporal_state::TRANSMITTED:
 			handler_transmitted(temporal);
 			break;
 		case Temporal_Object::temporal_state::RECEIVED_AT_SWITCH:
 			handler_received_at_switch(temporal);
 			break;
 		case Temporal_Object::temporal_state::SERVICING:
 			handler_servicing(temporal);
 			break;
 		default:
 			throw std::logic_error("Packet size not set.");
 	}
 } 

void Handler::handler_generated(Temporal_Object* temporal){
	return;
}

void Handler::handler_queued_at_src(Temporal_Object* temporal) {
	return;
}

void Handler::handler_transmitted(Temporal_Object* temporal) {
	return;
}

void Handler::handler_received_at_switch(Temporal_Object* temporal) {
	return;
}

void Handler::handler_servicing(Temporal_Object* temporal) {
	return;
}
