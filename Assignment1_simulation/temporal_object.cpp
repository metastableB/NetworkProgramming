/*
 * @author:metastableB
 * temporal_object.cpp
 */

#include "temporal_object.h"

Temporal_Object::Temporal_Object(Source* s, Packet* p,
	std::chrono::system_clock::time_point t,
	temporal_state st){

	source = s;
	packet = p;
	key = t;
	state = st;
}

Temporal_Object::Temporal_Object(Source* s, std::chrono::system_clock::time_point t, temporal_state st){
	source = s;
	packet = nullptr;
	key = t;
	state = st;
}
Temporal_Object::Temporal_Object(){

}
bool Temporal_Object::operator()(Temporal_Object* a, Temporal_Object* b){
	return a->key  >= b->key;
}