/*
main handler */
#include "source.h"
#include "packet.h"
#include "switch.h"
#include "handler.h"
#include "temporal_object.h"

// Weird name to prevent accidental modification
static int p_size_do_1234;
static bool p_size_set_do_1234 =  false;
static int p_no_sources_do_1234 = 0;
// REFER respective classes!
const bool& Packet::p_size_set(p_size_set_do_1234);
const int& Packet::p_size(p_size_do_1234);
const bool& Source::p_size_set(p_size_set_do_1234);
const int& Source::p_size(p_size_do_1234);
const bool& Switch::p_size_set(p_size_set_do_1234);
const int& Switch::p_size(p_size_do_1234);
const int& Handler::no_sources(p_no_sources_do_1234);
int Switch::round_robin = 0;

int main(){
	/* Details you need to start
	packet size
	sending rate of source
	bandwidth of link
	bandwidth of sink link
	queue size at sources
	queue size at switch
	Source type
	Switch type
	epoch for simulation start
	duration for simulation
	*/
	p_size_do_1234 = 100;
	p_size_set_do_1234 = true;
	p_no_sources_do_1234 = 2;
	std::cout << " Starting Simulation \n";
	// Packet sending rate, operation mode, num_sources
	Switch sw(200,Switch::switch_operating_mode::PACKET_SWITCHING,2);
	/* Packet sending rate, link bandwidth, max_q_size, connection type, burst size, burst_time_delt; */
	Source s(200,400,100,Source::connection_type::FIXED);
	Source s2(200,400,100,Source::connection_type::FIXED);
	//Handler::Handler(std::chrono::duration<long,std::nano> d, std::vector<Source> *s_l, Switch *s){
	std::vector<Source> sv;
	sv.push_back(s);
	sv.push_back(s2);
	Handler handler(std::chrono::seconds(1000),&sv,&sw);
	handler.simulate();
	return 0;
}