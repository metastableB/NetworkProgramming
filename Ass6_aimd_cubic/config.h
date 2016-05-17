#ifndef CONFIG_H
#define CONFIG_H

#define OUTPUT_FILE "./out.csv"
#define DATA_POINTS 100
#define MULTIPLIER 1
#define INCREMENTS 10
/* Everything in seconds (bps) */
#define SIM_DURATION std::chrono::seconds(700)
#define NUM_SOURCES	 1

#define PACKET_SIZE  5

#define SRC_SENDING_RATE 450 //bps
#define SRC_LINK_BW 600 //bps
#define SRC_MAX_Q_SIZE 400
#define SRC_CONNECTION_TYPE Source::connection_type::FIXED//FIXED/BURSTY
#define SRC_BURST_SIZE 500
#define SRC_BURST_TIME_DELTA std::chrono::microseconds(4000000)

#define SW_SERVICING_RATE 500
#define SW_SINK_BW SW_SERVICING_RATE
#define SW_OPERATING_MODE Switch::switch_operating_mode::PACKET_SWITCHING
#define SW_Q_SIZE_MAX 10000

#define DEFAULT_WINDOW 5
#define PACKET_DROP_PROBABILITY 0.1

/****************************************
************ LOGGING ********************
*****************************************/

/* Log the queueing delay vs the parameters on y */
#undef Q_DELAY_SRC_VS_Q_SIZE
#undef Q_DELAY_SRC_VS_P_SENDING_RATE
#undef Q_DELAY_SRC_VS_BURST_SIZE //ND
#undef Q_DELAY_SRC_VS_BURST_TIME_DELTA //ND

/* log outgoing queue vs switch queue on
low packet sending rate and low burst size
high sending rate and high burst size
*/
#undef OUT_QUEUE_VS_TIME
#undef SWITCH_QUEUE_VS_TIME

/* Packet loss vs q_size for both queues
for low packet sending rate and low burst size 
and high packet sending rate and high burst size 
*/
#undef P_LOST_SRC_VS_Q_SIZE
#undef P_LOST_SW_VS_Q_SIZE 

/* 
 * Average throughput at switch
 * for various lin speed to the sink
 * packet sending rate of the sources
 * number of sources
 */
#endif