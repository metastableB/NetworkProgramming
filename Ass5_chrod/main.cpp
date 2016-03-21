#include "./chord_node.h"
#include <iostream>


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

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#include <string>
#include <iostream>
#include <queue>

#define STDOUT 1
#define STDIN 0
#define BACKLOG 10
void* sthread_func(void*);
void* rthread_func(void *);

struct sthread_arg{
	std::string ip;
	int port;
};

struct rthread_args{
	int sockfd;
};

Chord_Node* chord;
/*
 * 1,5,4
 *
 */
int main(int argc, char* argv[]){
	if(argc < 4)
		return printf("Usage : host_ip host_port key [join_ip join_port]\n");
	Chord_Node x(argv[1],std::stoi(argv[2]),std::stoi(argv[3]));
	printf(KMAG"We are runnign on %s:%s key = %s\n",argv[1],argv[2],argv[3]);
	std::string ip = "";
	int port = -1;
	/* START NEW THREAD WITH SERVER */
	pthread_t ser;
	chord = &x;
	struct sthread_arg s;
	s.ip = argv[1];
	s.port = std::stoi(argv[2]);
	pthread_create(&ser,NULL,sthread_func,&s);
	if(argc == 6){
		port = std::stoi(argv[5]);
		ip = argv[4];
	}
	x.join(ip,port);
	while(1){
		std::cout << "\n1. Print Fingertable\n";
		std::cout << "2. Print Data\n";
		std::cout << "3. Add Data\n";
		int op;
		std::cin >> op;
		switch(op){
			case 1: 
				std::cout << KYEL;
				chord->test_print_debug();
				std::cout << KMAG;
				break;
			case 2: 
				std::cout << "\n\n";
				std::cout << KYEL << chord->get_data_csv() <<KMAG;
				std::cout << "\n";
				break;
			case 3: 
				std::cout << "Enter data: ";
				std::cin >> op;
				chord->add_data(op);
				std::cout << "\n\n";
				std::cout << KYEL << chord->get_data_csv() <<KMAG;
				std::cout << "\n";
				break;
			default:
				std::cout << "Unknown\n";
		}
	}
	pthread_join(ser,NULL);
	//x.test_random_finger();
	//std::cin >> ip;
	return 0;
}

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

void* sthread_func(void* x_) {
	sthread_arg* x = (sthread_arg*) x_;
	std::string ip = x->ip;
	int port = x->port;
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

	if ((rv = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
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

	printf("server: waiting for connections on [localhost:%d]\n",port);

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
#ifdef __DEBUG__
		printf("server: got connection from %s\n", s);
#endif
		pthread_t rthread;
		struct rthread_args rarg;
		rarg.sockfd = new_fd;
		if(pthread_create(&rthread, NULL, rthread_func, &rarg)) {
			fprintf(stderr, "Error creating thread\n");
			exit(1);
		}	
 	} 

	

	close(sockfd); // We dont listen anymore

}

void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

void tokenize(std::string req, std::vector<std::string> &token){
	int i = 0,j;
	j = req.find('|');
	token.push_back(req.substr(i,j-i));
	while(1){
		i = j+1;
		j = req.find('|',j+1);
		if(j == std::string::npos) {
			token.push_back(req.substr(i));
			break;
		} else token.push_back(req.substr(i,j-i));
	} 
}



/*
 * PROTOCOL DEFINITIONS
 *
 * << GET_CPF|k
 * >> POST_CPF|<cpf_ip>|<cpf_port>|<cpf_key>
 *
 * << UPDATE_PREDECESSOR|nodeip|port|key
 * >> UPDATED_PREDECESOR
 *
 * << UPDATE_FINGER_1|nodeip|port|key
 * >> UPDATED_FINGER_1
 *
 * >> UPDATE_FINGER_TABLE|ip|port|key|i
 * << UPDATED_FINGER_TABLE
 *
 * << GET_FINGER_1
 * >> POST_FINGER_1|<FINGER_1_ip>|<FINGER_1_port>|<FINGER_1_key>
 * 
 * >> FIND_SUCCESSOR|key
 * << FOUND_SUCCESSOR|ip|port|key
 */

std::string get_response(std::string req){
	std::vector<std::string> tokens;
	tokenize(req,tokens);
	/*
 	 * << GET_CPF|k
 	 * >> POST_CPF|<cpf_ip>|<cpf_port>|<cpf_key>
 	 */
	if(tokens[0] == "GET_CPF"){
		node_info n = chord->n_cpf(stoi(tokens[1]), chord->get_ip(), chord->get_port());
		return "POST_CPF|"+n.ip+"|"+std::to_string(n.port)+"|"+std::to_string(n.key);
	}
	/* >> FIND_SUCCESSOR|key
	 * << FOUND_SUCCESSOR|ip|port|key
	 */
	else if(tokens[0] == "FIND_SUCCESSOR"){
		node_info n = chord->n_find_successor(std::stoi(tokens[1]));
		return "FOUND_SUCCESSOR|" + n.ip + "|" + std::to_string(n.port) + "|" + std::to_string(n.key);
	}
	/*
	 * >> FIND_PREDECESSOR|key
 	 * << FOUND_PREDECESSOR|ip|port|key
 	 */
	else if(tokens[0] == "FIND_PREDECESSOR"){
		node_info n = chord->n_find_predecessor(std::stoi(tokens[1]));
		return "FOUND_PREDECESSOR|" + n.ip + "|" + std::to_string(n.port) + "|" + std::to_string(n.key);
	}
 	 /*
 	  * << UPDATE_PREDECESSOR|nodeip|port|key
 	  * >> UPDATED_PREDECESSOR
 	  */
	else if(tokens[0] == "UPDATE_PREDECESSOR"){
		node_info n(tokens[1],std::stoi(tokens[2]),std::stoi(tokens[3]));
		chord->set_predecessor(n);
		return "UPDATED_PREDECESSOR";
	}
	/*
	 * << GET_FINGER_1
 	 * >> POST_FINGER_1|<FINGER_1_ip>|<FINGER_1_port>|<FINGER_1_key>
 	 */
 	else if(tokens[0] == "GET_FINGER_1"){
 		node_info n = chord->get_finger(1);
 		return "POST_FINGER_1|" + n.ip + "|" + std::to_string(n.port) + "|" + std::to_string(n.key);
 	}

 	/*
	 * >> UPDATE_FINGER_TABLE|ip|port|key|i
	 * << UPDATED_FINGER_TABLE
	 */
	else if(tokens[0] == "UPDATE_FINGER_TABLE"){
		node_info n(tokens[1],std::stoi(tokens[2]),std::stoi(tokens[3]));
		chord->update_finger_table(n,std::stoi(tokens[4]));
		return "UPDATED_FINGER_TABLE";
	}
	/*
	 * >> ADD_DATA|data
	 * << ADDED_DATA
	 */
	else if(tokens[0] == "ADD_DATA"){
		chord->add_data(std::stoi(tokens[1]));
		return "ADDED_DATA";
	}
	/*
	 * >> MIGRATE_DATA
	 * << MIGRATED
	 */
	else if(tokens[0] == "MIGRATE_DATA"){
		chord->migrate_data();
		return "MIGRATED";
	}
}

void *rthread_func(void *p_){
	int sockfd = ((struct rthread_args*)p_)->sockfd;
	char buf[MAXDATASIZE];
	while(1){
		int bread;
		bread = recv(sockfd, buf, MAXDATASIZE - 1, 0);
		if(bread <= 0){
			//perror("recv");
			break;
		}
		buf[bread] = '\0';
		int t;
		std::string c = get_response(buf);
#ifdef __DEBUG__
		printf("_>> %s\n",buf);	
		std::cout << "_<< " << c << std::endl;
#endif
		if (send(sockfd, c.c_str(), (c).length(), 0) == -1){
			perror("send");	
			break;
		}
	}
	close(sockfd);  
}