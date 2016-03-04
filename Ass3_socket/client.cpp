/*
** client.c -- a stream socket client demo
*/

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

#include <iostream>
#include <queue>

#define STDOUT 1
#define STDIN 0
#define MAXDATASIZE 256
#define PORT "3490" // the port client will be connecting to 

void* rthread_func(void *);
struct rthread_args{
	int sockfd;
};

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]){
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure
	pthread_t rthread;
	struct rthread_args rarg;
	rarg.sockfd = sockfd;
	if(pthread_create(&rthread, NULL, rthread_func, &rarg)) {
		fprintf(stderr, "Error creating thread\n");
		exit(1);
	}
	while(1){
		int bread;
		bread = recv(sockfd, buf, MAXDATASIZE - 1, 0);
		if(bread <= 0){
			perror("recv");
			exit(1);
		}
		buf[bread] = '\0';
		printf("client: received '%s'\n",buf);	
	}
	/* we use two threads. One will read from STDIO and */

	/*
	while(i < 1001100){
		i++;
		sleep(1);
		if (FD_ISSET(STDIN, &readfds)){
        	std::cin >> msg;
        	q_sin.push(msg);
    	}
    	if(FD_ISSET(sockfd, &readfds)){
    		int bread;
    		bread = recv(sockfd, buf, MAXDATASIZE - 1, 0);
    		if(bread <= 0){
    			perror("recv");
    			exit(1);
    		}
    		buf[bread] = '\0';
    		q_skt.push(buf);
    	}
    	if(FD_ISSET(STDOUT, &writefds)){
    		while(!q_sin.empty()){
    			auto t = q_sin.front();
    			q_sin.pop();
    			std::cout << t << '\n';
    		}
    	}
    	if(FD_ISSET(sockfd, &writefds)){
    		while(!q_skt.empty()){
    			auto t = q_skt.front();
    			q_skt.pop();
    			if (send(sockfd, t.c_str(), t.length(), 0) == -1)
					perror("send");
    		}
    	}

	}
	printf("client: received '%s'\n",buf);*/

	close(sockfd);

	return 0;
}

void *rthread_func(void *p_){
	int sockfd = ((struct rthread_args*)p_)->sockfd;
	while(1){
		std::cout << "Reading\n";
		std::string s;
		std::cin >> s;
		if (send(sockfd, s.c_str(), s.length(), 0) == -1)
			perror("send");	
	}
}
