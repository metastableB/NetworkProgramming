/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <string>
#include <iostream>
#include <queue>

#define STDOUT 1
#define STDIN 0
#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold
#define MAXDATASIZE 256

void sigchld_handler(int s) {
	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // Bind to local

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	/* MAGIC CODE : No idea how this works */
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");


	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd == -1) {
		perror("accept");
		exit(1);
	}

	inet_ntop(their_addr.ss_family,
		get_in_addr((struct sockaddr *)&their_addr),
		s, sizeof s);
	printf("server: got connection from %s\n", s);

	close(sockfd); // We dont listen anymore

	/* here we have to both write and read from STDIN STDOUT and sockets */
	struct timeval tv;
    fd_set readfds, writefds;

    tv.tv_sec = 0;
    tv.tv_usec = 250000;

    FD_ZERO(&readfds);
    FD_SET(STDIN, &readfds);
    FD_SET(new_fd, &readfds);

    FD_ZERO(&writefds);
    FD_SET(STDOUT,&writefds);
    FD_SET(new_fd, &writefds);

    // don't care about writefds and exceptfds:
    select(new_fd+1, &readfds, &writefds, NULL, &tv);
    char buf[MAXDATASIZE];
    std::queue<std::string> q_sin, q_skt;
    std::string msg;
    while(1) {
    //std::cout << "enterdloop" << std::endl;
    	if (FD_ISSET(STDIN, &readfds)){
    //		std::cout << "reading\n" << std::endl;
        	std::cin >> msg;
        	q_sin.push(msg);
    	}
    	if(FD_ISSET(new_fd, &readfds)){
    		int bread;
    		bread = recv(new_fd, buf, MAXDATASIZE-1, 0);
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
    	if(FD_ISSET(new_fd, &writefds)){
    		while(!q_skt.empty()){
    			auto t = q_skt.front();
    			q_skt.pop();
    			if (send(new_fd, t.c_str(), t.length(), 0) == -1)
					perror("send");
    		}
    	}
    }
	close(new_fd);  
	return 0;
}

