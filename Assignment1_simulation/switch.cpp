#include "switch.h"

Switch::Switch(double psr,enum switch_operating_mode s, int num_sources){
	packet_servicing_rate = psr;
	this->num_sources = num_sources;
	set_servicing_time_delta();
	switch_op_mode = s;
	if(switch_op_mode == switch_operating_mode::PACKET_SWITCHING){

	} else 
		throw std::invalid_argument("Only Packet Switching works now");
}

Packet* Switch::arrival_handler(Packet* p){
	int index;
	if(switch_op_mode == switch_operating_mode::PACKET_SWITCHING)
		index = 0;
	else index = p->get_source_id();
	enqueue(p,index);
	return p;
}

Packet* Switch::service_packet(){
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
}

std::chrono::system_clock::time_point 
Switch::get_next_servicing_time_point(std::chrono::system_clock::time_point t){
	return t + servicing_time_delta;
}

Packet* Switch::enqueue(Packet* p,int i){
	if(q_size[i] >= q_size_max[i])
		p->set_packet_state(Packet::packet_state::LOST_SW);
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