#include "switch.h"

Switch::Switch(double psr,double sbw,enum switch_operating_mode s, int num_sources,int q_s_m){
	packet_servicing_rate = psr;
	this->num_sources = num_sources;
	sink_bw = sbw;
	set_servicing_time_delta();
	switch_op_mode = s;
	if(switch_op_mode == switch_operating_mode::PACKET_SWITCHING){
		master_q.resize(1);
		q_size.resize(1);
		q_size_max.resize(1);
		q_size_max[0] = q_s_m;
		q_size[0] = 0;
	} else {
		master_q.resize(num_sources);
		q_size.resize(num_sources);
		q_size_max.resize(num_sources);
		for(int i = 0 ; i < num_sources; i++){
			q_size_max[i] = q_s_m;
			q_size[i] = 0;
		}
	}
}

Switch::~Switch(){
	for(size_t i = 0; i < master_q.size();i++)
		for(size_t j = 0; j < master_q[i].size(); j++){
			auto p = master_q[j].front();
			master_q[j].pop();
			delete p;
		}
}

Packet* Switch::arrival_handler(Packet* p){
	/* Enqueues the arrived packet into the appropriate queue depending
	 * on the operating mode */
	int index;
	if(switch_op_mode == switch_operating_mode::PACKET_SWITCHING)
		index = 0;
	else index = p->get_source_id();
	enqueue(p,index);
	return p;
}

Packet* Switch::service_packet(){
	/* dequeues a packet for servicing from the source queues
	 * depending on the mode of operation and returns the dequeued
	 * packet
	 */
	int index;
	if(switch_op_mode == switch_operating_mode::PACKET_SWITCHING)
		index = 0;
	else {
		index = round_robin;
		round_robin = (round_robin + 1)%num_sources;
	}
	auto p = dequeue(index);
	if(p!=nullptr)
		p->set_packet_state(Packet::packet_state::HEAVEN);
	return p;
}

void Switch::set_servicing_time_delta(){
	servicing_time_delta = std::chrono::microseconds((long)(p_size*1000000/sink_bw));
	if(servicing_time_delta <= std::chrono::microseconds(0))
		throw std::runtime_error("Servicing time delta is negative at switch\n");
	#ifdef _DEBUG_
		std::cout <<"Servicing time delta for switch " << servicing_time_delta.count() << "\n";
	#endif
}

std::chrono::high_resolution_clock::time_point 
Switch::get_next_servicing_time_point(std::chrono::high_resolution_clock::time_point t){
	return t + servicing_time_delta;
}

Packet* Switch::enqueue(Packet* p,int i){
	if(q_size[i] >= q_size_max[i]){
		p->set_packet_state(Packet::packet_state::LOST_SW);
	}
	else {
		q_size[i]++;
		p->set_packet_state(Packet::packet_state::QUEUED_SW);
		master_q[i].push(p);
	}
	return p;
}

Packet* Switch::dequeue(int i){
	if(q_size[i] <= 0)
		return nullptr;
	auto p = master_q[i].front();
	master_q[i].pop();
	q_size[i]--;
	return p;
}

long Switch::get_0_queue_size(){
	return q_size[0];
}