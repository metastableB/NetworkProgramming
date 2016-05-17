#ifndef PACKET_H 
#define PACKET_H

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <map>
#include <set>	
#include <queue>
#include <climits>
#include <ctime>
#include <chrono>

class Packet {
	static const int& p_size;	
	static const bool& p_size_set;
	int source_id;
	int id;
	int seq_num;
	std::chrono::high_resolution_clock::time_point time_stamp_creation;
public:
	static int next_id;
	enum packet_state{
		GENERATED,
		QUEUED_SRC,
		LOST_SRC,
		DISPATCHED,
		QUEUED_SW,
		LOST_SW, // HELL!
		HEAVEN,	// HAHA!
	};

	enum packet_type{
		MSG,
		ACK,
	};
	enum packet_state p_state;
	enum packet_type p_type;

	Packet(int s_id, std::chrono::high_resolution_clock::time_point t, packet_type p_t = MSG);
	int get_source_id();
	void set_packet_state(packet_state s);
	int get_id();
	
	void set_queue_time_point(std::chrono::high_resolution_clock::time_point t);
	
	std::chrono::high_resolution_clock::time_point queue_time_point;
	std::chrono::duration<long,std::nano>
	get_queueing_delay(std::chrono::high_resolution_clock::time_point t);
	void set_packet_type(packet_type);
	void set_seq_num(int);
	int get_seq_num();
	bool is_packet_type(packet_type);
};
#endif