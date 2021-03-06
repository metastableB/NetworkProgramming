#ifndef SWITCH_H 
#define SWITCH_H

#include <stdexcept>
#include <queue>
#include "packet.h"
#include "config.h"

class Switch {
	static int round_robin;
	// Input in bps, handles in micro resolution
	double packet_servicing_rate = 0;
	std::chrono::duration<long long,std::micro> servicing_time_delta;
	double sink_bw;
	/* Vector of queues.
	 * If its TDM the size of the vector is N
	 * else its 1
	 */
	int num_sources;
	std::vector<std::queue<Packet*>> master_q;
	std::vector<int>q_size;
	std::vector<int>q_size_max;
	static const int& p_size;
	static const bool& p_size_set;

	void set_servicing_time_delta();
	Packet* enqueue(Packet* p, int i);
	Packet* dequeue(int i);
public:
	enum switch_operating_mode {
		TDM,
		PACKET_SWITCHING,
		NOT_SET,
	};
	switch_operating_mode switch_op_mode = NOT_SET;
	// Packet servicing rate,sink_bw, operation mode, num_sources
	Switch(double psr, double sink_bw,enum switch_operating_mode s,int num_sources = 1,int q_s_m = 3);
	~Switch();
	
	Packet* arrival_handler(Packet* p);
	Packet* service_packet();

	std::chrono::high_resolution_clock::time_point 
	get_next_servicing_time_point(std::chrono::high_resolution_clock::time_point t);
	long get_0_queue_size();
};
#endif