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
#include <list>
#include "packet.h"
#include "config.h"

 /*
  * Implementation notes.
  * Technically the source class is not as abstracted or encapsulated as you'd like.
  * The object should be able to govern the production and dispatching of packets on its
  * own without external interference but this is not the case here. 
  *
  * Oh well!
  */

class Source {
	int id;	
	 /*WARNING: In bps */
	double sending_rate;
	double dispatching_rate;
	double link_bw;
	bool is_transmitting = false;
	/* previous sending time + sending time delta = next sending time */
	std::chrono::duration<long,std::micro> sending_time_delta;
	/* previous dispatch time + dispatch time delta = next dispatch time delta */
	std::chrono::duration<long,std::micro> dispatching_time_delta;
	static const int& p_size;
	static const bool& p_size_set;
	
	/* Fow AIMD */
	int packets_sent;
	int next_seq_num;
	int next_to_ack;
	int def_window;
	int curr_window;
	int next_window;
	std::map<int,Packet*> seq_num_map;
	//std::map<int,int> ack_map;
	/* For BURSTY Connections */
	/* Number of packets send in each burst */
	double burst_size;
	/* Time difference between two bursts in milliseconds*/
	std::chrono::duration<long,std::micro> burst_time_delta;
	/* total number of packets sent in this burst */
	double no_packets_sent;

	std::list<Packet*> p_queue;
	long q_size;
	long q_size_max;
	Packet* enqueue(Packet* p);
	Packet* front();
	Packet* dequeue(Packet*);

	void calulate_dispatching_rate();
	void calculate_dispatching_time_delta();
	void calculate_sending_time_delta();
public:
	static int next_id;
	enum connection_type {
		FIXED,
		BURSTY	
	} c_type;
	
	Source();
	/* Packet sending rate, link bandwidth, max_q_size, connection type, burst size, burst_time_delt; */
	Source(double s_r, double link_bw, long max_q_size, enum connection_type x,
		int w_size, double b_sz = 0, 
		std::chrono::duration<long,std::micro> btd = std::chrono::duration<long,std::micro>(0));
	/* The above signature is why I feel C++ is a high maintanance lover! Okay yeah I know there are ways
	around this but still */
	Packet* generate_packet(std::chrono::high_resolution_clock::time_point t, Packet::packet_type p = Packet::packet_type::MSG);
	Packet* get_packet_for_dispatch();
	int handle_ack(Packet*);
	Packet* get_packet_for_retransmission(int seq_no);
	void update_window(bool);
	long get_queue_size();
	long get_max_queue_size();
	int get_id();	
	double get_sending_rate();
	void set_is_transmitting(bool b);
	bool get_is_transmitting();
	int get_curr_window();
	int get_source_id();

	std::chrono::duration<long,std::micro> 
		get_sending_time_delta();
	std::chrono::duration<long,std::micro> 
		get_dispatching_time_delta();
	std::chrono::high_resolution_clock::time_point 
		get_next_sending_time_point(std::chrono::high_resolution_clock::time_point t);
	std::chrono::high_resolution_clock::time_point 
		get_next_arrival_time_point(std::chrono::high_resolution_clock::time_point t);
	std::chrono::high_resolution_clock::time_point 
		get_next_dispatching_time_point(std::chrono::high_resolution_clock::time_point t);
	
};
#endif 