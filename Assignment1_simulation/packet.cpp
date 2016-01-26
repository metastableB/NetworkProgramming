/*
 * @author:metastableB
 * packet.cpp
 * 
 */
#include "packet.h"

Packet::Packet(int s_id){			
	if(!p_size_set)
		throw std::logic_error("Packet size not set.");

	this->source_id = s_id;
	time_stamp_creation = std::chrono::system_clock::now();
	p_state = packet_state::GENERATED;
}

void Packet::set_packet_state(packet_state s){
	p_state = s;
}