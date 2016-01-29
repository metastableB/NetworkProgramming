/*
 * @author:metastableB
 * temporal_object.cpp
 */

#include "temporal_object.h"

Temporal_Object::Temporal_Object(Source* s, Packet* p,
	std::chrono::high_resolution_clock::time_point t,
	temporal_state st){

	source = s;
	packet = p;
	key = t;
	state = st;
}

Temporal_Object::Temporal_Object(Source* s, std::chrono::high_resolution_clock::time_point t, temporal_state st){
	source = s;
	packet = nullptr;
	key = t;
	state = st;
}
Temporal_Object::Temporal_Object(){

}
bool Temporal_Object::operator()(Temporal_Object* a, Temporal_Object* b){
	return (a->key - b->key).count() >= 0;
}

std::string Temporal_Object::get_string_state(){
	switch(state) {
		case temporal_state::GENERATE:
			return "GENERATE";
		case temporal_state::QUEUED_SRC:
			return "QUEUED_SRC";
		case temporal_state::TRANSMIT:
			return "TRANSMIT";
		case temporal_state::RECEIVED_SWITCH:
			return "RECEIVED_SWITCH";
		case temporal_state::SERVICING:
			return "SERVICING";
	}
	return "";
}
