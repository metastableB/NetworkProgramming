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
	*/
	p_size_do_1234 = 10;
	p_size_set_do_1234 = true;
	p_no_sources_do_1234 = 2;
	std::cout << " LOL \n";
	std::cout << p_size_do_1234 << p_size_set_do_1234;
	Switch sw(2,Switch::switch_operating_mode::PACKET_SWITCHING);
	Source s(3,3,3,Source::connection_type::FIXED);
	return 0;
}