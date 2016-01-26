#include "source.h"
int Source::next_id = 0;
/* Packet sending rate, link bandwidth, q_size, connection type, burst size, burst_time_delt; */
Source::Source(double p_s_r, int l_bw, int q_sz,
		enum connection_type c_t, int b_sz, 
		std::chrono::duration<int,std::milli> btd ) {
	id = next_id++;	
	link_bw = l_bw;
	q_size_max = q_sz;
	q_size = 0;
	calulate_p_dispatching_rate();
	if(c_t == connection_type::BURSTY){
		if(b_sz <= 0 || btd <= std::chrono::milliseconds(0))
			throw std::invalid_argument("Invalid burst size and burst duration.");
		burst_size = b_sz;
		burst_time_delta = btd;
	}
}
/* Generator creates a new packet and envokes the dispatcher with the 
 * new packet */
Packet* Source::packet_generator(){
	Packet* p = new Packet(this->id);
	//TODO : e.log("Packet creation time");
	return packet_dispatcher(p);
}
/* Dispatcher services packets from the queue at
 * the dispatching rate. Changes the packet status
 * indicator to dispatched. 
 */
Packet* Source::packet_dispatcher(Packet* p = nullptr){
	if(p!= nullptr)
		enqueue(p);
	//TODO: lOG Packet status, i.e if lost of queued
	//TODO: if it is ready to be dispatched and then send
	//TODO: We have to mod this for busty connections as well
	Packet* p_ = dequeue();
	p_->set_packet_state(Packet::packet_state::DISPATCHED);
	return p_;
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
void Source::calulate_p_dispatching_rate(){
	p_dispatchig_rate = link_bw/p_sending_rate;
}
