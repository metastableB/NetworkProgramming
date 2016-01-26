/*
* @author:metastableB
* source.cpp
*/ 

#include "source.h"

int Source::next_id = 0;
// Packet sending rate, link bandwidth, max_q_size, connection type, burst size, burst_time_delt
Source::Source(double p_s_r, double l_bw, long max_q_sz,
		enum connection_type c_t, double b_sz, 
		std::chrono::duration<double,std::milli> btd ) {
	id = next_id++;	
	link_bw = l_bw;
	q_size_max = max_q_sz;
	q_size = 0;
	calulate_dispatching_rate();
	calculate_dispatching_time_delta();
	calculate_sending_time_delta();

	if(c_t == connection_type::BURSTY){
		if(b_sz <= 0 || btd <= std::chrono::milliseconds(0))
			throw std::invalid_argument("Invalid burst size and burst duration.");
		burst_size = b_sz;
		burst_time_delta = btd;
	}
}


// Private methods
void Source::calulate_dispatching_rate(){
	dispatchig_rate = link_bw / p_size;
}

void Source::calculate_dispatching_time_delta(){
	dispatch_time_delta = std::chrono::duration<double,std::milli> (p_size/link_bw);
}
void Source::calculate_sending_time_delta(){
	sending_time_delta = std::chrono::duration<double,std::milli> (p_size/sending_rate);
}

long Source::get_queue_size(){ return q_size;}
long Source::get_max_queue_size() { return q_size_max;}

Packet* Source::enqueue(Packet* p){
	if(q_size >= q_size_max)
		p->set_packet_state(Packet::packet_state::LOST_SRC);
	else {
		q_size++;
		p->set_packet_state(Packet::packet_state::QUEUED_SRC);
		p_queue.push(p);
	}
	return p;
}

Packet* Source::dequeue(){
	if(q_size <= 0)
		return nullptr;
	auto p = p_queue.front();
	p_queue.pop();
	q_size--;
	return p;
}