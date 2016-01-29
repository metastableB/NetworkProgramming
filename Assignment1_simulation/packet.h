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
	enum packet_state p_state;
	Packet(int s_id);
	int get_source_id();
	void set_packet_state(packet_state s);
	int get_id();
};
#endif