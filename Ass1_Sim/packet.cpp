/*
 * @author:metastableB
 * packet.cpp
 * 
 */
#include "packet.h"

Packet::Packet(int s_id,std::chrono::high_resolution_clock::time_point t){			
	if(!p_size_set)
		throw std::logic_error("Packet size not set.");
	id = next_id++;
	this->source_id = s_id;
	time_stamp_creation = t;
	p_state = packet_state::GENERATED;
#ifdef _DEBUG_
	std::cout << source_id << " Packet generated " << id << " \n";
#endif
}

void Packet::set_packet_state(packet_state s){
	p_state = s;
}

int Packet::get_source_id(){
	return source_id;
}

int Packet::get_id(){
	return id;
}
void Packet::set_queue_time_point(std::chrono::high_resolution_clock::time_point t){
	queue_time_point = t;
}

std::chrono::duration<long,std::nano>
Packet::get_queueing_delay(std::chrono::high_resolution_clock::time_point t){
	return (t - queue_time_point);
}
