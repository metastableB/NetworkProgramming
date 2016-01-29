/*
 * @author:metastableB
 * temporal_object.h
 */

#ifndef TEMPORAL_OBJECT_H
#define TEMPORAL_OBJECT_H

#include "source.h"
#include "packet.h"
#include "switch.h"

class Temporal_Object {
	/* this is where all the magic happens! */
	
public:
	// Associated packet
	Packet* packet;
	// The source the associated packet originated from 
	Source* source;
	/* key :
	WARNING : DONOT update key while within the priority queue */
	/* The handler gets the previous state and has to do the transition */
	enum temporal_state {
		GENERATE,
		QUEUED_SRC,
		TRANSMIT,
		RECEIVED_SWITCH,
		SERVICING,
	} state;
	std::chrono::high_resolution_clock::time_point key;
	
	Temporal_Object(Source* s, Packet* p,
		std::chrono::high_resolution_clock::time_point t,
		temporal_state st);
	// Grin : just because I dont understand C++ properly
	Temporal_Object();
	Temporal_Object(Source* s, std::chrono::high_resolution_clock::time_point t, temporal_state st);
	std::string get_string_state();
	bool operator()(Temporal_Object*,Temporal_Object*);
};
#endif