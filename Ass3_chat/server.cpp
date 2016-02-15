/* A simple server in the internet domain using TCP
The port number is passed as an argument */
#include "definitions.h"

using namespace std;
/* Explain system error codes */
void error(const char *msg){
    printf("%s",msg);
    std::cout << std::endl;
}

struct server_rw {
	bool kill_thread;
	int sockfd;
	struct server_ *srv;
} s_rw;

void* pt_select_rw(void* i){
	struct server_rw *rw = (struct server_rw*) i ;
	struct server_ *srv =  rw->srv;
	int n;
	char buffer[256];
	std::queue<std::string> inq,outq;
	
	struct timeval tv;
	fd_set readfds,writefds;
	tv.tv_sec = 20;
	tv.tv_usec = 0;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_SET(STDIN, &readfds);
	FD_SET(rw->sockfd,&readfds);
	FD_SET(rw->sockfd,&writefds);
	FD_SET(STDOUT,&writefds);

	std::string std_in,sock_in;

	while(!rw->kill_thread){
		select(rw->sockfd + 1, &readfds, NULL,NULL, &tv);

		if(FD_ISSET(STDIN,&readfds)){
			std::cout << "READ from stdin" << std::endl;
			std::cin >> std_in;
			if(std_in[0] == '#') {
				std_in = "#";
				outq.push(std_in);
				break;
			}else {
				std_in = srv->user_id + ": " + std_in;
				outq.push(std_in);
			}
		}

	    if(FD_ISSET(rw->sockfd, &readfds)){
	    	std::cout << "read from soc" << std::endl;
			memset(buffer,0,256);
			n = read(rw->sockfd,buffer,255);
			if (n < 0) {
			    error("ERROR reading from socket");
			    srv->status = 1;
			    rw->kill_thread = true;
			  	pthread_exit(&srv->status);
			}
			if(buffer[0] == '#') break;
			inq.push(std::string(buffer));
		}

		if(FD_ISSET(rw->sockfd,&writefds)){
			std::cout << "write from soc" << std::endl;
			std::cout << "Blocking on write server" << std::endl;
			std::string buff = outq.front();
			outq.pop();
			n = write(rw->sockfd,buff.c_str(),strlen(buff.c_str()));
			if(buff == "#") break;
			if (n < 0) {
				error("ERROR writing to socket");
				srv->status = 1;
				rw->kill_thread = true;
				pthread_exit(&srv->status);
			}
		}

		if(FD_ISSET(STDOUT,&writefds)){
			std::cout << "write from stdout" << std::endl;
			auto buff = inq.front();
			if(buff == "#") break;
			inq.pop();
			std::cout << buff;
		}	
	}
	rw->kill_thread = true;
	FD_ZERO(&readfds);
	return nullptr;
}

void* pt_write_skt_s(void *i){
	std::cout << "write\n";
	struct server_rw *rw = (struct server_rw*) i ;
	struct server_ *srv =  rw->srv;
	char buffer[256];
	int n;
	while(!rw->kill_thread){
		bzero(buffer,256);
		fgets(buffer,245,stdin);
		std::string buff(buffer);
		//if(buffer[0] == '#')
		//	break;
		buff = srv->user_id + ": " + buff;
		n = write(rw->sockfd,buff.c_str(),strlen(buff.c_str()));
		if (n < 0) {
			error("ERROR writing to socket");
			srv->status = 1;
			rw->kill_thread = true;
			pthread_exit(&srv->status);
		}
	}
	rw->kill_thread = true;
	return nullptr;
}

void* pt_read_skt_s(void* i){
	std::cout << "read\n";
	struct server_rw *rw = (struct server_rw*) i;
	struct server_ *srv =  rw->srv;
	char buffer[256];
	int n;
	while(!rw->kill_thread){
		memset(buffer,0,256);
		n = read(rw->sockfd,buffer,255);

		if (n < 0) {
		    error("ERROR reading from socket");
		    srv->status = 1;
		    rw->kill_thread = true;
		  	pthread_exit(&srv->status);
		}
		if(buffer[0] == '#') break;
		printf("%s",buffer);
	}
	rw->kill_thread = true;
	return nullptr;
}

void *server(void *i) {
    // File descriptors into the file disc. table
    // storing the values returned by sockfdet and accept
    int sockfd, newsockfd;
    struct server_ *srv= (struct server_*) i;
    int portno = srv->portno;
    sem_t *sem_server = (sem_t*)srv->sem_server;
    sem_t *sem_driver = (sem_t*)srv->sem_driver;
    // Stores length of address of client needed for accept system code
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    // Opening a file descriptor 
    // AF_UNIX for unix domain.
    // SOCK_STREAM is used for stream messages SOCK_DGRAM for datagram
    // final argument is protocol.0 indicates default - TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
        srv->status = 1;
        sem_post(sem_driver);
        pthread_exit(&srv->status);
    }
    // Initializing the buffer to all zeros.
    memset((char *) &serv_addr, 0,sizeof(serv_addr));

    /*struct sockaddr_in {

    short   sin_family; // must be AF_INET 
    u_short sin_port;
    struct  in_addr sin_addr;
    char    sin_zero[8]; // Not used, must be zero 
    }; */

    serv_addr.sin_family = AF_INET;
    // INADDR_ANY : Symbolic constant that gets the host address
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //htons() which converts a port number in host byte order to a port number in network byte order.
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr * ) &serv_addr,
        sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    	srv->status = 1;
        sem_post(sem_driver);
        pthread_exit(&srv->status);
    }
    /*The first argument is the socket file descriptor, and the second 
    is the size of the backlog queue, i.e., the number of connections 
    that can be waiting while the process is handling a particular 
    connection. This should be set to 5, the maximum size permitted 
    by most systems.*/
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    srv->status = 0;
   	sem_post(sem_driver);
   	sem_wait(sem_server);
   	std::cout << "Server intialization done. Bound to port " << portno << std::endl;

    // Accept blocks this process till a successful connection is established
    // A new files descriptor is defined for the client process.
	newsockfd = accept(sockfd,
    	(struct sockaddr *) &cli_addr, 
    	&clilen);
	// If we are not making an outgoing connection, it means we are accepting an incoming
	if(srv->status != 3)
		srv->status = 2;
    if(newsockfd < 0) {
        error("ERROR on accept");
        srv->status = 1;
        pthread_exit(&srv->status);
    } 

   s_rw.kill_thread = false;
   s_rw.sockfd = newsockfd;
   s_rw.srv = srv;
   
   /* A client connected to this server and we stoped our client
   	  we start a new client thread and let it connect to the
   	  incoming client */
   char buffer[256];
   std::string buff;
   int n;
   if(srv->status == 2){
   	   std::cout << "Making outgoing connection\n";
	   pthread_t pt_r,pt_w;
	   printf("Reading port_no and host name\n");
	   int pono;
	   std::string hostname;
	   memset(buffer,0,256);
	   n = read(newsockfd,buffer,255);
	   std::stringstream ss(buffer);
	   ss >> portno >> hostname;
	   std::cout << hostname << ":" << portno << std::endl;
	   client_thread(portno, hostname, srv->user_id,0);
	}
   /*
   pthread_create(&pt_w,NULL,pt_write_skt_s,(void *)&s_rw);
   pthread_join(pt_w,nullptr);
   pthread_create(&pt_r,NULL,pt_read_skt_s,(void *)&s_rw);
   pthread_create(&pt_r,NULL,pt_select_rw,(void*)&s_rw);
   pthread_join(pt_r,nullptr);*/
   printf("Connected\n");
	while(true){
		memset(buffer,0,256);
		n = 0;
		while(n ==0 )
			n = read(newsockfd,buffer,255);
		if (n < 0) {
			error("ERROR reading from socket");
			srv->status = 1;
			pthread_exit(&srv->status);
		}
		if(buffer[0] == '#') break;
		std::cout << buffer;
   }
   close(newsockfd);
   close(sockfd);
   return nullptr;
}
