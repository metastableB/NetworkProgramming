

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
#include <map>

#define STDOUT 1
#define STDIN 0

#define BACKLOG 10	 // how many pending connections queue will hold
#define MAXDATASIZE 256

#define PORT "3500"

void* rthread_func(void *);
void* authThread(void *);

struct rthread_args{
	int sockfd;
};

class authInfo{
	public:
	std::string username;
	std::string password;
	std::string ip ="";
	std::string port = "";
	bool isLoggedIn = false;
	authInfo(){

	}
	authInfo(std::string a, std::string b){
		username = a;
		password = b;
	}
};

std::map<std::string, authInfo> userlist;


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

int main(int argc, char* argv[]) {
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

	/* Manually adding the user information */
	userlist["u_001"] = authInfo("u_001","p_001");
	userlist["u_002"] = authInfo("u_002","p_002");
	userlist["u_003"] = authInfo("u_003","p_003");
	userlist["u_004"] = authInfo("u_004","p_004");

	printf("server: waiting for connections...\n");

	char buf[MAXDATASIZE];
	sin_size = sizeof their_addr;
	while (1) {
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			exit(1);
		}
		inet_ntop(their_addr.ss_family,
		get_in_addr((struct sockaddr *)&their_addr),
		s, sizeof s);
		printf("server: got connection from %s\n", s);
		authThread(&new_fd);
 	} 
	
	return 0;
}


void *rthread_func(void *p_){
	int sockfd = ((struct rthread_args*)p_)->sockfd;
	while(1){
		std::string s;
		getline(std::cin,s);
		if (send(sockfd, s.c_str(), s.length(), 0) == -1)
			perror("send");	
	}
}

void *authThread(void *p_){
	int new_fd = *((int *)p_);
	char buf[256];
	int bread;
	bread = recv(new_fd, buf, MAXDATASIZE - 1, 0);
	if(bread <= 0){
		perror("recv");
		close(new_fd);  
		exit(1);
	}
	buf[bread] = '\0';
	printf("%s\n",buf);
	std::string auth(buf);
	auth  = auth + '|';
	int n1 = 0,n2 = auth.find('|',0);
	if(!(auth.substr(0,n2) == "AUTHENTICATE")) {
		std::cout << auth.substr(0,n2+1) << " Garbage\n";
		std::string s  = "GARBAGE MESSAGE RECEIVED";
		if (send(new_fd, s.c_str(), s.length(), 0) == -1)
			perror("send");	
		close(new_fd);  
		return NULL;
	}
	n1 = n2+1;
	std::vector<std::string> tok;
	n2 = auth.find('|',n1);
	
	while(n2 != std::string::npos){
		tok.push_back(auth.substr(n1,n2-n1));
		n1 = n2+1;
		n2 = auth.find('|',n1);
	}
	for(std::string s: tok)
		std::cout << s << std::endl;
	if(auth.size() >= 2 && userlist[tok.at(0)].password == tok.at(1)){
		std::string s;
		if(userlist[tok.at(0)].isLoggedIn){
			s  = "ALREADY_AUTHENTICATED";
			return NULL;
		}
		else {
			s  = "AUTHENTICATED|";
			userlist[tok.at(0)].isLoggedIn = true;
			userlist[tok.at(0)].ip = tok.at(2);
			userlist[tok.at(0)].port = tok.at(3);

			for(auto it = userlist.begin(); it != userlist.end(); it++){
				if(it->second->isLoggedIn && it->first != tok.at(0)){
					s = s + it->second.username + "|";
					s = s + it->second.ip + "|";
					s = s + it->second.port + "|";
				}
			}
		}
		if (send(new_fd, s.c_str(), s.length(), 0) == -1)
			perror("send");	
	} else {
		std::string s  = "INVALID_CREDENTIALS";
		if (send(new_fd, s.c_str(), s.length(), 0) == -1)
			perror("send");	
	}
	close(new_fd);  
}