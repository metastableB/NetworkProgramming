/*
main handler */
#include "source.h"
#include "packet.h"
#include "switch.h"

// Weird name to prevent accidental modification
static int p_size_do_1234;
static bool p_size_set_do_1234 =  false;

const bool& Packet::p_size_set(p_size_set_do_1234);
const int& Packet::p_size(p_size_do_1234);
const bool& Source::p_size_set(p_size_set_do_1234);
const int& Source::p_size(p_size_do_1234);

int main(){
	p_size_do_1234 = 0;
	p_size_set_do_1234 = true;
	std::cout << p_size_do_1234 << p_size_set_do_1234;
	Switch sw(2,Switch::switch_operating_mode::PACKET_SWITCHING);
	Source s(3,3,3,Source::connection_type::FIXED);
	return 0;
}