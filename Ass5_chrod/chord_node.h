#ifndef CHORD_NODE_H
#define CHORD_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cmath>
#include <string>
#include <map>
#include <vector>
#include <iostream>

#define M_M 3 // The value m, as defined in the paper
#define POW_M 8 // 2^_M
#define MAXDATASIZE 256

#define _PAUSE_() char x120; std::cin >> x120;

struct node_info {
	std::string ip;
	int port;
	int key;
	node_info(std::string i, int p, int k){
		ip = i;
		port = p;
		key = k;
	}
	node_info(){
		ip = "";
		port = -1;
		key = -1;
	}
};

class Chord_Node {
	std::string ip;
	int port;
	int key;
	std::vector<int> data;
	struct node_info predecessor;
	std::map<int,std::pair<int,struct node_info>> finger_table;
	
	int start(int k);
	bool belongs_e_i(int,int,int);
	bool belongs_e_e(int,int,int);
	bool belongs_i_e(int,int,int);
	void tokenize(std::string, std::vector<std::string>&);
	struct node_info get_cpf(int key);
	struct node_info get_finger_1(std::string, int);
	void update_successor(node_info&);
	void update_predecessor(node_info&);

	void *get_in_addr(struct sockaddr *sa);
	int get_socket_fd(std::string ip, int port);
	int send_msg(std::string msg,int);
	std::string receive_msg(int sockfd);
	std::string proto_exec(std::string msg, int);
	
	void init_finger_table(std::string i, int p);
	void update_others();
	void update_finger_table();
public:
	Chord_Node(std::string,int,int);	
	std::string get_ip();
	int get_port();
	int get_key();
	// Returns a ',' separated data sequence
	std::string get_data_csv();
	struct node_info get_predecessor();
	struct node_info get_finger(int);


	void add_data(int);
	/* we manually set the key to prevent hash collisions since we are
	 * working in a limited key space, namely (0, MOD_M)
	 */
	void set_key(int);
	void set_port(int);
	void set_ip(std::string);
	void set_predecessor(node_info);

	void update_finger_table(node_info,int);
	struct node_info n_find_successor(int key);
	struct node_info n_find_successor(int key, std::string ip, int port);
	struct node_info n_find_predecessor(int key);
	struct node_info n_find_predecessor(int key, std::string ip, int port);
	struct node_info n_cpf( int key, std::string ip, int port);
	// Node joins
	void join(std::string, int);
	// Testing
	int test_send(std::string, std::string,int);
	int test_proto_exec(std::string, std::string, int);
	int test_random_finger();
	int test_print_debug();
};

#endif //CHORD_NODE_H