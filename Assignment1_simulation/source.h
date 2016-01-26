/*
 * @author:metastableB
 * source.cpp
 * 
 */
#ifndef SOURCE_H
#define SOURCE_H

#include <vector>
#include <queue>
#include <iostream>
#include <climits>
#include <chrono>
#include <stdexcept>
#include "packet.h"

class Source {
	int id;	
	 /*WARNING: In bps */
	long sending_rate;
	long dispatchig_rate;
	long link_bw;
	/* previous sending time + sentind time delta = next sending time */
	std::chrono::duration<long,std::milli> sending_time_delta;
	/* previous dispatch time + dispatch time delta = next dispatch time delta */
	std::chrono::duration<long,std::milli> dispatch_time_delta;
	static const int& p_size;
	static const bool& p_size_set;
	
	
	/* For BURSTY Connections */
	/* Number of packets send in each burst */
	int burst_size;
	/* Time difference between two bursts in milliseconds*/
	std::chrono::duration<int,std::milli> burst_time_delta;
	
	std::queue<Packet*> p_queue;
	int q_size;
	int q_size_max;
	Packet* enqueue(Packet* p);
	Packet* dequeue();

	void calulate_dispatching_rate();
	void calculate_dispatch_time_delta();
	void calculate_sending_time_delta();
public:
	static int next_id;
	enum connection_type {
		FIXED,
		BURSTY	
	};
	/* Packet sending rate, link bandwidth, connection type, burst size, burst_time_delt; */
	Source(double p_s_r, int link_bw,int q_size, enum connection_type x,
		int b_sz = 0, std::chrono::duration<int,std::milli> btd = std::chrono::seconds(0));
	/* Generator creates a new packet and envokes the dispatcher with the 
	 * new packet */
	Packet* packet_generator();
	/* Dispatcher if called with a new packet tries to enqueu the packet and if
	 * the time is right tries to send the packet. We may have to maintain a last send
	 * time. 
	 */
	Packet* packet_dispatcher(Packet *p);

};
#endif 