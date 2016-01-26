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
	double sending_rate;
	double dispatchig_rate;
	double link_bw;
	/* previous sending time + sentind time delta = next sending time */
	std::chrono::duration<double,std::milli> sending_time_delta;
	/* previous dispatch time + dispatch time delta = next dispatch time delta */
	std::chrono::duration<double,std::milli> dispatch_time_delta;
	static const int& p_size;
	static const bool& p_size_set;
	
	/* For BURSTY Connections */
	/* Number of packets send in each burst */
	double burst_size;
	/* Time difference between two bursts in milliseconds*/
	std::chrono::duration<double,std::milli> burst_time_delta;
	/* total number of packets sent in this burst */
	double no_packets_sent;

	std::queue<Packet*> p_queue;
	long q_size;
	long q_size_max;
	Packet* enqueue(Packet* p);
	Packet* dequeue();

	void calulate_dispatching_rate();
	void calculate_dispatching_time_delta();
	void calculate_sending_time_delta();
public:
	static int next_id;
	enum connection_type {
		FIXED,
		BURSTY	
	};
	/* Packet sending rate, link bandwidth, max_q_size, connection type, burst size, burst_time_delt; */
	Source(double s_r, double link_bw, long max_q_size, enum connection_type x,
		double b_sz = 0, std::chrono::duration<double,std::milli> btd = std::chrono::duration<double,std::milli>(0));

	long get_queue_size();
	long get_max_queue_size();	
	
};
#endif 