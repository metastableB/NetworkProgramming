/*
* @author:metastableB
* source.cpp
*/ 

#include "source.h"

int Source::next_id = 0;
Source::Source(){
	/* For defalut vector initialization. 
	 * Why the does a pointer to a vector require constructor anyway ?
	 * Update: April 3: 
	 * Figured it out -_-
	 */
}
// Packet sending rate, link bandwidth, max_q_size, connection type, burst size, burst_time_delt
Source::Source(double p_s_r, double l_bw, long max_q_sz,
		enum connection_type c_t, int w,double b_sz,
		std::chrono::duration<long,std::micro> btd ) {
	if(p_size_set == false)
		throw std::runtime_error("Packet size not set!\n");
	sending_rate = p_s_r;
	id = next_id++;	
	link_bw = l_bw;
	q_size_max = max_q_sz;
	q_size = 0;
	def_window = w;

	next_to_ack = 1;
	next_seq_num = 0;
	def_window = DEFAULT_WINDOW;
	curr_window = DEFAULT_WINDOW;
	next_window = DEFAULT_WINDOW;
	packets_sent = 0;

	calulate_dispatching_rate();
	calculate_dispatching_time_delta();
	calculate_sending_time_delta();

	if(c_t == connection_type::BURSTY){
		if(b_sz <= 0 || btd <= std::chrono::microseconds(0))
			throw std::invalid_argument("Invalid burst size and burst duration.\n");
		burst_size = b_sz;
		burst_time_delta = btd;
		no_packets_sent = 0;
		if(sending_time_delta >= burst_time_delta){
			std::cout << "Sending time delta " <<  sending_time_delta.count() 
				<< " burst time delta " << burst_time_delta.count() << std::endl;
			throw std::logic_error("The sending time delta >= burst time delta\n");
		}
	} else c_type = connection_type::FIXED;
}


Packet* Source::generate_packet(std::chrono::high_resolution_clock::time_point t, Packet::packet_type p_t){
	// Packet GENERATED
	Packet* p = new Packet(this->id,t);
	p->set_packet_type(p_t);
	// packet sequencing done at dispatch
	// Packet either in Packet::LOST_SRC or Packet::QUEUED_SRC
	enqueue(p);
	p->queue_time_point = t;
	return p;
}

Packet* Source::get_packet_for_dispatch(){
	auto p = front();
	if(p == nullptr) return nullptr;
	if(packets_sent >= curr_window) return nullptr;
	dequeue(p);
	p->set_seq_num(next_seq_num);
	seq_num_map[next_seq_num] = p;
	//ack_map[next_seq_num] = 0;
	packets_sent++;
	next_seq_num = (next_seq_num + 1)% curr_window;
	p->set_packet_state(Packet::packet_state::DISPATCHED);	
	return p;
}
Packet* Source::get_packet_for_retransmission(int seq_no){
	return seq_num_map[seq_no];
}
int Source::handle_ack(Packet* p){
	// we require retransmission
	if( p->get_seq_num() < next_to_ack){
		update_window(false);
		return p->get_seq_num();
	}
	//std::cout << "received ack " << p->get_seq_num() << ""
	next_to_ack = p->get_seq_num() + 1;
	if(next_to_ack == curr_window)
		update_window(true);
	return -1;
}


void Source::update_window(bool t){
	if(!t){
		next_window = (curr_window + 1)/2;
		return;
	}
	/* We have completed a window */
	next_window++;
	curr_window = next_window;
	for(auto &i:seq_num_map)
		i.second = nullptr;
	std::cout << "\nWindow\t"<< curr_window << "\n" << std::endl;
	next_to_ack = 1;
	next_seq_num = 0;
	packets_sent = 0;
}

std::chrono::high_resolution_clock::time_point 
Source::get_next_sending_time_point(std::chrono::high_resolution_clock::time_point t){
	if(c_type == connection_type::FIXED)
		return t + sending_time_delta;
	else {
		if(no_packets_sent == burst_size){
			no_packets_sent = 0;
			return t + burst_time_delta;
		}
		no_packets_sent++;
		return t + sending_time_delta;
	}
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
int Source::get_curr_window(){
	return curr_window;
}


int Source::get_source_id(){
	return id;
}
Packet* Source::enqueue(Packet* p){
	if(q_size >= q_size_max)
		p->set_packet_state(Packet::packet_state::LOST_SRC);
	else {
		q_size++;
		p->set_packet_state(Packet::packet_state::QUEUED_SRC);
		p_queue.push_back(p);
	}
	return p;
}

Packet* Source::dequeue(Packet* p){
	if(q_size <= 0)
		return nullptr;
	auto it = p_queue.begin();
	for(; it != p_queue.end(); it++)
		if((*it)->get_id() == p->get_id() ) break;
	if(it == p_queue.end()) throw std::logic_error("Packet to dequeu not found");
	p_queue.erase(it);
	q_size--;
	return p;
}

Packet* Source::front(){
	return p_queue.front();
}