/*
* @author:metastableB
* source.cpp
*/ 

#include "source.h"

int Source::next_id = 0;
Source::Source(){
	/* For defalut vector initialization. 
	 * Why the fuck does a pointer to a vector require constructor anyway ?
	 */
}
// Packet sending rate, link bandwidth, max_q_size, connection type, burst size, burst_time_delt
Source::Source(double p_s_r, double l_bw, long max_q_sz,
		enum connection_type c_t, double b_sz, 
		std::chrono::duration<long,std::micro> btd ) {
	if(p_size_set == false)
		throw std::runtime_error("Packet size not set!\n");
	sending_rate = p_s_r;
	id = next_id++;	
	link_bw = l_bw;
	q_size_max = max_q_sz;
	q_size = 0;
	calulate_dispatching_rate();
	calculate_dispatching_time_delta();
	calculate_sending_time_delta();

	if(c_t == connection_type::BURSTY){
		if(b_sz <= 0 || btd <= std::chrono::microseconds(0))
			throw std::invalid_argument("Invalid burst size and burst duration.\n");
		burst_size = b_sz;
		burst_time_delta = btd;
	} else c_type = connection_type::FIXED;
}


Packet* Source::generate_packet(){
	// Packet GENERATED
	Packet* p = new Packet(this->id);
	// Packet either in Packet::LOST_SRC or Packet::QUEUED_SRC
	enqueue(p);
	return p;
}

Packet* Source::get_packet_for_dispatch(){
	auto p = dequeue();
	if(p != nullptr)
		p->set_packet_state(Packet::packet_state::DISPATCHED);
	return p;
}

std::chrono::high_resolution_clock::time_point 
Source::get_next_sending_time_point(std::chrono::high_resolution_clock::time_point t){
	if(c_type == connection_type::FIXED)
		return t + sending_time_delta;
	else throw std::invalid_argument("BURST NOT YET SUPPORTED!\n");
	return t;
}

std::chrono::high_resolution_clock::time_point 
Source::get_next_dispatching_time_point(std::chrono::high_resolution_clock::time_point t){
	return t + dispatching_time_delta + std::chrono::nanoseconds(5);
}

std::chrono::high_resolution_clock::time_point 
Source::get_next_arrival_time_point(std::chrono::high_resolution_clock::time_point t){
	return t + dispatching_time_delta;
}



// Auxiliary methods
void Source::calulate_dispatching_rate(){
	dispatching_rate = link_bw / p_size;
}

// Some very poor design choices caused by a very poor understanding of the templating
// system and chrono library caused this.
void Source::calculate_dispatching_time_delta(){
	dispatching_time_delta = std::chrono::microseconds((long)(p_size*1000000/link_bw));
#ifdef _DEBUG_ 
	std::cout << "Dispatching time delta for " << this->id <<
	  " " << dispatching_time_delta.count() << '\n';
#endif
}
void Source::calculate_sending_time_delta(){
	sending_time_delta = std::chrono::microseconds((long)(p_size*1000000/sending_rate));
#ifdef _DEBUG_ 
	std::cout << "Sending time delta for " << this->id <<
	  " " << sending_time_delta.count() << '\n';
#endif
}

long Source::get_queue_size(){
	return q_size;
}

long Source::get_max_queue_size() {
	return q_size_max;
}

double Source::get_sending_rate(){
	return sending_rate; 
}

std::chrono::duration<long,std::micro> 
Source::get_sending_time_delta() {
	return sending_time_delta; 
}

std::chrono::duration<long,std::micro> 
Source::get_dispatching_time_delta() {
	return dispatching_time_delta; 
}

void Source::set_is_transmitting(bool b){
	is_transmitting = b;
}

bool Source::get_is_transmitting(){
	return is_transmitting;
}
int Source::get_id(){
	return this->id;
}
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
