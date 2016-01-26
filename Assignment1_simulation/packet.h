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
/* weird name so that you dont accidentaly mess around*/

class Packet {
	/* Taken as input from user at begining */
	static const int& p_size;	
	static const bool& p_size_set;
	int source_id;
	std::chrono::system_clock::time_point time_stamp;

public:
	enum packet_state{
		GENERATED,
		QUEUED_SRC,
		LOST_SRC,
		DISPATCHED,
		QUEUED_SW,
		LOST_SW,
	};
	enum packet_state p_state;
	Packet(int s_id);
	void set_packet_state(packet_state s);
};

#endif