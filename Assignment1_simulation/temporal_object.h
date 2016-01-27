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
	// Associated packet
	Packet* packet;
	// The source the associated packet originated from 
	Source* source;
public:
	/* key :
	WARNING : DONOT update key while within the priority queue */
	/* The handler gets the previous state and has to do the transition */
	enum temporal_state {
		GENERATED,
		QUEUED_AT_SRC,
		TRANSMITTED,
		RECEIVED_AT_SWITCH,
		SERVICING,
	} state;

	std::chrono::system_clock::time_point key;
	Temporal_Object(Source* s, Packet* p,
		std::chrono::system_clock::time_point t,
		temporal_state st);

	bool operator()(Temporal_Object*,Temporal_Object*);
};
#endif