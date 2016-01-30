#ifndef CONFIG_H
#define CONFIG_H

#define OUTPUT_FILE "./out.csv"
#define DATA_POINTS 100
#define MULTIPLIER 1.1
#define INCREMENTS 50
/* Everything in seconds (bps) */
#define SIM_DURATION std::chrono::seconds(300)
#define NUM_SOURCES	 10

#define PACKET_SIZE  100

#define SRC_SENDING_RATE 400 //bps
#define SRC_LINK_BW 200 //bps
#define SRC_MAX_Q_SIZE 10
#define SRC_CONNECTION_TYPE Source::connection_type::FIXED//FIXED/BURSTY
#define SRC_BURST_SIZE 10
#define SRC_BURST_TIME_DELTA std::chrono::microseconds(800000)

#define SW_SERVICING_RATE 200
#define SW_SINK_BW 200
#define SW_OPERATING_MODE Switch::switch_operating_mode::PACKET_SWITCHING
#define SW_Q_SIZE_MAX 5

/****************************************
************ LOGGING ********************
*****************************************/

/* Log the queueing delay vs the parameters on y */
#undef Q_DELAY_SRC_VS_Q_SIZE
#define Q_DELAY_SRC_VS_P_SENDING_RATE
#undef Q_DELAY_SRC_VS_BURST_SIZE
#undef Q_DELAY_SRC_VS_BURST_TIME_DELTA

/* log outgoing queue vs switch queue on
low packet sending rate and low burst size
high sending rate and high burst size
*/
#undef OUT_QUEUE_VS_SWITCH_QUEUE

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