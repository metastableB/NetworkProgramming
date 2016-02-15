#include <string>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <queue>
#include <sstream>

extern void* server(void* i);
extern void* client(void* i);
extern void error(const char *msg);
extern void client_thread(int portno,std::string hostname, std::string user_id,int status);

#define STDIN 0
#define STDOUT 1
struct server_ {
	int status;
	int portno;
	sem_t *sem_server;
	sem_t *sem_driver;
	std::string user_id;
	/* incase we are waiting for a response */
	std::string responder_addr;
};

struct client_{
	int status;
	int status_con;
	int portno;
	std::string hostname;
	std::string user_id;
	sem_t *sem_client;
	sem_t *sem_driver;
	int bound_port;
};