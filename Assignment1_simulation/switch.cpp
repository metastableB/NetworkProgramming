#include "switch.h"

Switch::Switch(double psr,enum switch_operating_mode s){
	packet_servicing_rate = psr;
	if(s == switch_operating_mode::PACKET_SWITCHING){

	} else 
		throw std::invalid_argument("Only Packet Switching works now");
}

Packet* Switch::arrival_handler_PS(Packet* p){
	/* THe packet switchig method. we try to enque */
	enqueue(p);
	if(p->p_state == Packet::packet_state::LOST_SRC);
		//TODO: LOG
	return p;
}

void Switch::service_packet(){

}

Packet* Switch::enqueue(Packet* p){
	if(q_size >= q_size_max)
		p->set_packet_state(Packet::packet_state::LOST_SRC);
	else {
		q_size++;
		p->set_packet_state(Packet::packet_state::QUEUED_SRC);
		master_q.push(p);
	}
	return p;
}

Packet* Switch::dequeue(){
	if(q_size <= 0)
		return nullptr;
	auto p = master_q.front();
	master_q.pop();
	q_size--;
	return p;
}