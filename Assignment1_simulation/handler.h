/*
* @author:metastableB
* handler.h
*/
#ifndef HANDLER_H
#define HANDLER_H

#include <queue>
#include <vector>
#include <stdexcept>

#include "temporal_object.h"
#include "source.h"
#include "switch.h"
/*
 *
 * MAGIC/ LOGIC
 *
 * We have the following points in ime
  - GENERATED: A new packet was just generated and it must be either queued
  		or transmitted before pushing to event queue. This is sort of a placeholder
  		state.
  - QUEUED_SRC : A New packet was just queued and accoring to a scheduled servicing
  		time we will transmit this. New generation object to be scheduled.
  - TRANSMITTING: We are performing a transmiision. We set is_transmitting. Also, 
  		if this state was directly reached without going through QUEUED_SRC, we need
  		to schedule the next packet GENERATED as well.
  		Scheduling the packet transmission for this sorce is also done here.
  - RECEIVED_AT_SRC:
  		Packet queued and we wait for the servicing event. TODO :
  		figure out more here especially when the queue is empty.
  - SERVICING: A packet is selected and serviced. Next servicing event is scheduled.
  		for the first case we require a start boolen to start the service for the first
  		time so that subsequent scheduling occurs.
  */

class Handler {
	std::vector<Source> source_list;
	bool is_servicing = false;
	Switch sw;
	static const int& no_sources;
	// Handlers
public:
	void handler_generated(Temporal_Object*);
	void handler_queued_at_src(Temporal_Object*);
	void handler_transmitted(Temporal_Object*);
	void handler_received_at_switch(Temporal_Object*);
	void handler_servicing(Temporal_Object*);

	std::priority_queue<Temporal_Object*,std::vector<Temporal_Object*>,Temporal_Object> event_queue;
	/* Creates N sources and generates packet for them, then these packets are transmitted
	initiating the process */
	void initialize();
	/* works on switch case according to big comment on top */
	void transition();
};
#endif


