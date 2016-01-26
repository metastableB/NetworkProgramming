#ifndef SWITCH_H 
#define SWITCH_H

#include <stdexcept>
#include <queue>
#include "packet.h"
/* We are going to implement packet switching for now.
  Each packet is going to get queued in to the common queue whose size 
  we will need to take as input.

  There will only be one instance of a switch class. we need a globally 
	The driver will call the enqueue handler with the pacet and the queue number 
	if required.

  */
class Switch {
	/* Milliseconds */
	double packet_servicing_rate = 0;
	std::queue<Packet*> master_q;
	int q_size;
	int q_size_max;
public:
	enum switch_operating_mode {
		TDM,
		PACKET_SWITCHING,
		NOT_SET,
	};
	switch_operating_mode s = NOT_SET;
	Switch(double psr,enum switch_operating_mode s);
	Packet* enqueue(Packet* p);
	Packet* dequeue();

	Packet* arrival_handler_TDM(Packet* p, int index);
	Packet* arrival_handler_PS(Packet* p);
	/* at the servicing rate, called by handler,
	   if time is right, service the packet */
	void service_packet();
};
#endif