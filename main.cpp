/*
main handler */
#include "source.h"
#include "packet.h"
#include "switch.h"
#include "handler.h"
#include "temporal_object.h"
#include "config.h"

// Weird name to prevent accidental modification
int Packet::next_id = 0;
static int p_size_do_1234;
static bool p_size_set_do_1234 =  false;
static int p_no_sources_do_1234 = NUM_SOURCES;
// REFER respective classes!
const bool& Packet::p_size_set(p_size_set_do_1234);
const int& Packet::p_size(p_size_do_1234);
const bool& Source::p_size_set(p_size_set_do_1234);
const int& Source::p_size(p_size_do_1234);
const bool& Switch::p_size_set(p_size_set_do_1234);
const int& Switch::p_size(p_size_do_1234);
const int& Handler::no_sources(p_no_sources_do_1234);
int Switch::round_robin = 0;

void initialize_globals();
void initialize_switch(Switch*&);
void initialize_source(std::vector<Source>& vc);
	
void packet_lost_src_vs_q_size();
void packet_lost_sw_vs_q_size();
void q_delay_src_vs_q_size();
void q_delay_src_vs_p_sending_rate();
void q_delay_src_vs_burst_size();
void dummy_test();

int main(){
	std::cout << "Starting Simulation \n";
	#ifdef _DEBUG_
	std::cout << " All time durations measured in micro seconds\n";
	#endif
	
	#ifdef P_LOST_SRC_VS_Q_SIZE
		packet_lost_src_vs_q_size();
	#elif defined(Q_DELAY_SRC_VS_Q_SIZE)
		q_delay_src_vs_q_size();
	#elif defined(Q_DELAY_SRC_VS_P_SENDING_RATE)
		q_delay_src_vs_p_sending_rate();
	#elif defined(Q_DELAY_SRC_VS_BURST_SIZE)
		q_delay_src_vs_burst_size();
	#elif defined(P_LOST_SW_VS_Q_SIZE)
		packet_lost_sw_vs_q_size();
	#else
		dummy_test();
	#endif
	//delete sw;
	std::cout << "\nWarning : Suspicious memory dealloc: Memory leaks?\n";
	return 0;
}

void packet_lost_src_vs_q_size(){
	std::cout << "Logging Packet Lost at src vs Queue Size\n";
	FILE* fp;
	fp = fopen(OUTPUT_FILE,"w");
	long max_q = SRC_MAX_Q_SIZE;

	if(SRC_CONNECTION_TYPE == Source::connection_type::BURSTY ||
		SW_OPERATING_MODE == Switch::switch_operating_mode::TDM)
		throw std::runtime_error("Cannot simulate in BURSTY/TDM\n");
	
	for(int i = 0; i < DATA_POINTS; i++) {
		Source::next_id = 0;
		if(i%10 == 0)
			std::cout << "Pass " << i << std::endl;
		initialize_globals();
		Switch* sw = nullptr;
		initialize_switch(sw);
		std::vector<Source> v_src;
		/* initializing source */
		max_q = max_q + INCREMENTS;
		for(int i = 0; i < NUM_SOURCES; i++){
			v_src.push_back(
				Source(SRC_SENDING_RATE,
				SRC_LINK_BW,
				max_q,
				SRC_CONNECTION_TYPE,
				SRC_BURST_SIZE,
				SRC_BURST_TIME_DELTA
				)
			);
		}
		Handler handler(SIM_DURATION,
			&v_src,
			sw);
		handler.simulate();
		fprintf(fp,"%ld\t%ld\n",max_q,handler.num_packets_lost_src);
	}
	fclose(fp);
	std::string args = std::string(OUTPUT_FILE) + " Source_queue_size " +"Number_of_Packets_Lost";
	std::string title = " Link_bw:_"+std::to_string(SRC_LINK_BW)+"bps,_Sending_rate_" + std::to_string(SRC_SENDING_RATE)+"bps";
	std::string img_name = " packet_lost_src_vs_q_size";
	std::string command = "python ./plotter.py " + args + title + img_name;
	std::system(command.c_str());
}

void q_delay_src_vs_q_size(){
	std::cout << "Logging average queuing delay at src vs Queue Size\n";
	FILE* fp;

	if(SRC_CONNECTION_TYPE == Source::connection_type::BURSTY ||
		SW_OPERATING_MODE == Switch::switch_operating_mode::TDM)
		throw std::runtime_error("Cannot simulate in BURSTY/TDM\n");
	
	fp = fopen(OUTPUT_FILE,"w");
	long max_q = SRC_MAX_Q_SIZE;
	for(int i = 0; i < DATA_POINTS; i++) {
		Source::next_id = 0;
		if(i%10 == 0)
			std::cout << "Pass " << i << std::endl;
		initialize_globals();
		Switch* sw = nullptr;
		initialize_switch(sw);
		std::vector<Source> v_src;
		/* initializing source */
		max_q = max_q + INCREMENTS;
		for(int i = 0; i < NUM_SOURCES; i++){
			v_src.push_back(
				Source(SRC_SENDING_RATE,
				SRC_LINK_BW,
				max_q,
				SRC_CONNECTION_TYPE,
				SRC_BURST_SIZE,
				SRC_BURST_TIME_DELTA
				)
			);
		}
		Handler handler(SIM_DURATION,
			&v_src,
			sw);
		handler.simulate();
		fprintf(fp,"%ld\t%ld\n",max_q,handler.average_queuing_delay.count()/1000000);
	}
	fclose(fp);
	std::string args = std::string(OUTPUT_FILE) + " Source_queue_size " +"Average_queuing_delay[ms]";
	std::string title = " Link_bw:_"+std::to_string(SRC_LINK_BW)+"bps,_Sending_rate_" + std::to_string(SRC_SENDING_RATE)+"bps";
	std::string img_name = " q_delay_src_vs_q_size";
	std::string command = "python ./plotter.py " + args + title + img_name;
	std::system(command.c_str());
}


void q_delay_src_vs_p_sending_rate(){
	std::cout << "Logging average queuing delay at src vs packet sending rate\n";
	FILE* fp;
	
	if(SRC_CONNECTION_TYPE == Source::connection_type::BURSTY ||
		SW_OPERATING_MODE == Switch::switch_operating_mode::TDM)
		throw std::runtime_error("Cannot simulate in BURSTY/TDM\n");
	
	fp = fopen(OUTPUT_FILE,"w");
	long max_p_s_r = SRC_SENDING_RATE;
	for(int i = 0; i < DATA_POINTS; i++) {
		Source::next_id = 0;
		if(i%10 == 0)
			std::cout << "Pass " << i << std::endl;
		initialize_globals();
		Switch* sw = nullptr;
		initialize_switch(sw);
		std::vector<Source> v_src;
		/* initializing source */
		max_p_s_r = max_p_s_r + INCREMENTS;
		for(int i = 0; i < NUM_SOURCES; i++){
			v_src.push_back(
				Source(max_p_s_r,
				SRC_LINK_BW,
				SRC_MAX_Q_SIZE,
				SRC_CONNECTION_TYPE,
				SRC_BURST_SIZE,
				SRC_BURST_TIME_DELTA
				)
			);
		}
		Handler handler(SIM_DURATION,
			&v_src,
			sw);
		handler.simulate();
		fprintf(fp,"%ld\t%ld\n",max_p_s_r,handler.average_queuing_delay.count()/1000000);
	}
	fclose(fp);
	std::string args = std::string(OUTPUT_FILE) + " Packet_sending_rate[bps] " +"Average_queuing_delay[ms]";
	std::string title = " Link_bw:_"+std::to_string(SRC_LINK_BW)+"bps,_Queue_Size_" + std::to_string(SRC_MAX_Q_SIZE);
	std::string img_name = " q_delay_src_vs_p_sending_rate";
	std::string command = "python ./plotter.py " + args + title + img_name;
	std::system(command.c_str());
}

void q_delay_src_vs_burst_size() {
	std::cout << "Logging average queuing delay at src vs burst size\n";
	FILE* fp;
	
	if(SRC_CONNECTION_TYPE == Source::connection_type::FIXED ||
		SW_OPERATING_MODE == Switch::switch_operating_mode::TDM)
		throw std::runtime_error("Cannot simulate in FIXED/TDM\n");
	
	fp = fopen(OUTPUT_FILE,"w");
	long max_bs = SRC_BURST_SIZE;
	
	for(int i = 0; i < DATA_POINTS; i++) {
		Source::next_id = 0;
		if(i%10 == 0)
			std::cout << "Pass " << i << std::endl;
		initialize_globals();
		Switch* sw = nullptr;
		initialize_switch(sw);
		std::vector<Source> v_src;
		/* initializing source */
		max_bs = max_bs + INCREMENTS;
		for(int i = 0; i < NUM_SOURCES; i++){
			v_src.push_back(
				Source(SRC_SENDING_RATE,
				SRC_LINK_BW,
				SRC_MAX_Q_SIZE,
				SRC_CONNECTION_TYPE,
				max_bs,
				SRC_BURST_TIME_DELTA
				)
			);
		}
		Handler handler(SIM_DURATION,
			&v_src,
			sw);
		handler.simulate();
		fprintf(fp,"%ld\t%ld\n",max_bs,handler.average_queuing_delay.count()/1000000);
	}
	fclose(fp);
	std::string args = std::string(OUTPUT_FILE) + " Burst_size " +"Average_queuing_delay[ms]";
	std::string title = " Link_bw:_"+std::to_string(SRC_LINK_BW)+"bps,_Queue_Size_" + std::to_string(SRC_MAX_Q_SIZE);
	title += ",_Sending_rate_"+std::to_string(SRC_SENDING_RATE);
	std::string img_name = " q_delay_src_vs_burst_size";
	std::string command = "python ./plotter.py " + args + title + img_name;
	std::system(command.c_str());

}


void packet_lost_sw_vs_q_size(){
	std::cout << "Logging Packet Lost at Switch vs Queue Size\n";
	FILE* fp;
	fp = fopen(OUTPUT_FILE,"w");

	if(SRC_CONNECTION_TYPE == Source::connection_type::BURSTY ||
		SW_OPERATING_MODE == Switch::switch_operating_mode::TDM)
		throw std::runtime_error("Cannot simulate in BURSTY/TDM\n");
	
	long max_sw_q = SRC_MAX_Q_SIZE;
	for(int i = 0; i < DATA_POINTS; i++) {
		Source::next_id = 0;
		if(i%10 == 0)
			std::cout << "Pass " << i << std::endl;
		initialize_globals();

		max_sw_q = max_sw_q + INCREMENTS;
		
		Switch* sw = nullptr;
		sw = new Switch(SW_SERVICING_RATE,
		SW_SINK_BW,
		SW_OPERATING_MODE,
		NUM_SOURCES,
		max_sw_q);

		std::vector<Source> v_src;
		initialize_source(v_src);
		Handler handler(SIM_DURATION,
			&v_src,
			sw);
		handler.simulate();
		fprintf(fp,"%ld\t%ld\n",max_sw_q,handler.num_packets_lost_sw);
	}
	fclose(fp);
	std::string args = std::string(OUTPUT_FILE) + " Switch_queue_size " +"Number_of_Packets_Lost";
	std::string title = " Link_bw:_"+std::to_string(SRC_LINK_BW)+"bps,_Servicing_rate_" + std::to_string(SW_SERVICING_RATE)+"bps";
	std::string img_name = " packet_lost_sw_vs_q_size";
	std::string command = "python ./plotter.py " + args + title + img_name;
	std::system(command.c_str());
}


void dummy_test(){
	std::cout << "Dummy simulation\n";
	initialize_globals();
	Switch* sw = nullptr;
	initialize_switch(sw);
	std::vector<Source> v_src;
	initialize_source(v_src);

	Handler handler(SIM_DURATION,
		&v_src,
		sw);

	handler.simulate();
}
void initialize_globals() {
	p_size_do_1234 = PACKET_SIZE;
	p_size_set_do_1234 = true;
	p_no_sources_do_1234 = NUM_SOURCES;
}

void initialize_switch(Switch* &sw){
	sw = new Switch(SW_SERVICING_RATE,
		SW_SINK_BW,
		SW_OPERATING_MODE,
		NUM_SOURCES);
}

void initialize_source(std::vector<Source>& v_src){
		/* Packet sending rate, link bandwidth, max_q_size, connection type, burst size, burst_time_delt; */
	for(int i = 0; i < NUM_SOURCES; i++){
		v_src.push_back(
			Source(SRC_SENDING_RATE,
			SRC_LINK_BW,
			SRC_MAX_Q_SIZE,
			SRC_CONNECTION_TYPE,
			SRC_BURST_SIZE,
			SRC_BURST_TIME_DELTA
			)
		);
	}
}
