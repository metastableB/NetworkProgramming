/*
* @author:metastableB
* handler.h
*/
#ifndef HANDLER_H
#define HANDLER_H

#include <queue>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <cstdio>

#include "temporal_object.h"
#include "source.h"
#include "switch.h"
#include "config.h"
/*
 *
 * MAGIC/ LOGIC
 *
 * We have the following points in ime
  - GENERATE: A new packet must be generated and it must be either queued
  		or transmitted before pushing to event queue. This is sort of a placeholder
  		state.
  - QUEUED_SRC : A New packet was just queued and according to a scheduled servicing
  		time we will transmit this. New generation object to be scheduled.
  - TRANSMITTING: We are performing a transmission. We set is_transmitting. Also, 
  		if this state was directly reached without going through QUEUED_SRC, we need
  		to schedule the next packet GENERATED as well.
  		Scheduling the packet transmission for this source is also done here.
  - RECEIVED_AT_SRC:
  		Packet queued and we wait for the servicing event. TODO :
  		figure out more here especially when the queue is empty.
  - SERVICING: A packet is selected and serviced. Next servicing event is scheduled.
  		for the first case we require a start boolean to start the service for the first
  		time so that subsequent scheduling occurs.
  */
/* 
 * Class API requirements
 * 
 * The class runs a simulation on a set of preinitialzied sources
 * for a specific duration. The switch which is involved should also
 * be preinitialized.
 * 
 * The class constructor requires that a vector of Sources be defined
 * and a reference to that vector be passed.
 * Similarly a Switch reference is also required.
 */
class Handler {
	// Output file for write
	FILE * fp;
	std::vector<Source> *source_list;
	bool is_servicing = false;
	Switch *sw;
	static const int& no_sources;

	std::chrono::duration<long,std::micro> simulation_duration;
	std::chrono::high_resolution_clock::time_point epoch;
	
	// Handlers
	void handler_generate(Temporal_Object*);
	void handler_queued_src(Temporal_Object*);
	void handler_transmit(Temporal_Object*);
	void handler_received_switch(Temporal_Object*);
	void handler_servicing(Temporal_Object*);
	/* Creates N sources and generates packet for them, then these packets are transmitted
	initiating the process */
	void initialize();
	/* works on switch case according to big comment on top */
	void transition(Temporal_Object*);
public:
	std::priority_queue<Temporal_Object*,std::vector<Temporal_Object*>,Temporal_Object> event_queue;
	Handler(std::chrono::duration<long,std::micro> d, std::vector<Source> *s_l, 
		Switch *s,bool in_handler_write = false);
	~Handler();
	void simulate();
	void print_queue_status();

	/* Variables for logging purposes */
	long num_packets_lost_src;
	/* this is used to calculate a running average and is not what the name suggests */
	long num_packets_queued_src;
	std::chrono::duration<long,std::nano> average_queuing_delay;
	long num_packets_lost_sw;
};
#endif


