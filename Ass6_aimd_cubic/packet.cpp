/*
 * @author:metastableB
 * packet.cpp
 * 
 */
#include "packet.h"

Packet::Packet(int s_id,std::chrono::high_resolution_clock::time_point t, packet_type p_t){			
	if(!p_size_set)
		throw std::logic_error("Packet size not set.");
	id = next_id++;
	this->source_id = s_id;
	time_stamp_creation = t;
	p_type = p_t;
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

void Packet::set_packet_type(packet_type t){
	p_type = t;
}

bool Packet::is_packet_type(packet_type p){
	return (p_type == p);
}

void Packet::set_seq_num(int s){
	seq_num = s;
}

int Packet::get_seq_num(){
	return seq_num;
}