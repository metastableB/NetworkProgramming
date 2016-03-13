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

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#define STDOUT 1
#define STDIN 0
#define PORT "3501"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold
#define MAXDATASIZE 256

std::string USERNAME;
std::string make_response(std::string);
void tokenize(std::string, std::vector<std::string>&);
sql::ResultSet* runStatement(std::string&, struct sql_conn*, std::string&);
int runUpdate(std::string&, struct sql_conn*, std::string&);
void logOut();
void setLoggedIn();

struct sql_conn{
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
};

void* rthread_func(void *);
struct rthread_args{
	int sockfd;
};

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

	char buf[MAXDATASIZE];
	sin_size = sizeof their_addr;

	while (1) {
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			exit(1);
		}

		int pid = fork();
   	    if (pid < 0)
     		perror("ERROR on fork");
        if (pid == 0) {
        	close(sockfd);
     		break;
   		}
   		else
     		close(new_fd);
 	} 

	inet_ntop(their_addr.ss_family,
		get_in_addr((struct sockaddr *)&their_addr),
		s, sizeof s);
	printf("server: got connection from %s\n", s);

	close(sockfd);
	/* 
	pthread_t rthread;
	struct rthread_args rarg;
	rarg.sockfd = new_fd;
	if(pthread_create(&rthread, NULL, rthread_func, &rarg)) {
		fprintf(stderr, "Error creating thread\n");
		exit(1);
	}**/
	while(1){
		int bread;
		bread = recv(new_fd, buf, MAXDATASIZE - 1, 0);
		if(bread <= 0){
			if(USERNAME != ""){
				std::cout << "LOGOUT " <<USERNAME << std::endl;
				logOut();
			}
			perror("recv");
			close(new_fd);
			exit(1);
		}
		buf[bread] = '\0';
		printf(">> %s\n",buf);	
		std::string s = make_response(buf);
		std::cout << "<< " << s << "\n" << std::endl ;
		if(s.length() == 0) continue;
		if (send(new_fd, s.c_str(), s.length(), 0) == -1)
			perror("send");	
	}
	close(new_fd);  
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

sql::ResultSet* runStatement(std::string &s, struct sql_conn* conn_, std::string &err) {
	try {
		conn_->driver = get_driver_instance();
		conn_->con = conn_->driver->connect("tcp://127.0.0.1:3306", "root", "midsemscomingnow");
		conn_->con->setSchema("chatApp");
		conn_->stmt = conn_->con->createStatement();
		//std::cout << "Runing " << s << std::endl;
		conn_->res = conn_->stmt->executeQuery(s); // replace with your statement
		//while (res->next()) {
		//std::cout << "\t... MySQL replies: ";
		//std::cout << res->getString(1) << std::endl;
		//}
		return conn_->res;
	} catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line "
		<< __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		err = e.what();
		return nullptr;
	}
}

int runUpdate(std::string &s, struct sql_conn* conn_, std::string &err) {
	try {
		conn_->driver = get_driver_instance();
		conn_->con = conn_->driver->connect("tcp://127.0.0.1:3306", "root", "midsemscomingnow");
		conn_->con->setSchema("chatApp");
		conn_->stmt = conn_->con->createStatement();
		int i = conn_->stmt->executeUpdate(s); // replace with your statement
		//while (res->next()) {
		//cout << "\t... MySQL replies: ";
		//cout << res->getString(1) << " " << res->getString(2) << " " << res->getString(3) << endl;
		//}
		return i;
	} catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line "
		<< __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		err = e.what();
		return -1;
	}
}


/* The connection object is only initialized
 * IT does not have any controll structure moves.
 * Make sure all communicatin to the authentication server goes through here.
 *
 * Mark protocol rules here
 *
 * AUTHENTICATE|username|password
 * >> AUTHENTICATE_SUCCESSS or AUTHENTICATE_FAILURE
 *
 * REGISTER|username|password
 * >> REGISTER_SUCCESS or REGISTER_FAILURE
 *
 * ADD_FRIEND|friendname
 * >> GET_FRIEND_LIST | ADD_FRIEND_FAILURE
 *
 * GET_FRIEND_LIST|username
 * >> FRIENDLIST|[username|ip|port]*
 *
 * POST_MY_IP|username|ip|port
 * >> POST_MY_IP_SUCCESS | POST_MY_IP_FAILURE
 *
 */
std::string make_response(std::string req){
	std::vector<std::string> token;
	tokenize(req,token);
	sql_conn *t = new sql_conn();

	if(token[0] == "AUTHENTICATE"){
		std::string e,s = "SELECT password FROM auth where username = '"+token[1]+"'";
		sql::ResultSet* res = runStatement(s,t,e);
		if(res == nullptr){
			delete t;
			return "AUTHENTICATE_FAILURE|"+e;
		}
		else{
			while(res->next()){
				//std::cout << res->getString(1) << " vs " << token[2] << std::endl;
				if(res->getString(1) == token[2]){
					USERNAME = token[1];
					delete t;
					setLoggedIn();
					return "AUTHENTICATE_SUCCESS";
				}
			}
			delete t;
			return "AUTHENTICATE_FAILURE|Invalid credentials";
		}

	} 

	if(token[0] == "REGISTER"){
		if(token.size() < 3){
			delete t;
			return "REGISTER_FAILURE|Too small arguemtns";
		}
		std::string e,s = "INSERT INTO auth VALUES(\""+token[1]+"\",";
		s += "\""+token[2]+"\",1,\"\",\"\");";
		std::cout << s << std::endl;
		int res = runUpdate(s,t,e);
		if(res == -1){
			delete t;
			return "REGISTER_FAILURE|" + e;
		}
		USERNAME = token[1];
		delete t;
		return "REGISTER_SUCCESS";
	} 

	if(token[0] == "ADD_FRIEND"){
		std::cout << "ere" << std::endl;
		if(token.size() < 2){
			delete t;
			return "ADD_FRIEND_FAILURE|Too small arguemtns";
		}
		std::string tmp,t2 = "SELECT 1 from auth where username = \""+token[1]+"\"";
		sql::ResultSet* r = runStatement(t2,t,tmp);
		if(!r->next()){
			delete t;
			return "ADD_FRIEND_FAILURE|No user with that name";
		}
		std::string e,s = "INSERT INTO friendList VALUES(\""+USERNAME+"\",\""+token[1]+"\")";
		std::cout << s;
		int res = runUpdate(s,t,e);
		if(res == -1){
			delete t;
			return "ADD_FRIEND_FAILURE|" + e;
		}
		s = "INSERT INTO friendList VALUES(\""+token[1]+"\",\""+USERNAME+"\")";
		res = runUpdate(s,t,e);
		if(res == -1){
			delete t;
			return "ADD_FRIEND_FAILURE|" + e;
		}
		token[0] = "GET_FRIEND_LIST";
	} 

	if(token[0] == "GET_FRIEND_LIST"){
		std::string e,s = "SELECT username,ip,port FROM auth where";
		s += " username in (SELECT friend_name FROM friendList where username = \""+USERNAME+"\")";
		std::cout << s << std::endl;
		sql::ResultSet* res = runStatement(s,t,e);
		if(res == nullptr){
			delete t;
			return "GET_FRIEND_LIST_FAILURE|"+e;
		}
		else{
			std::string p = "FRIENDLIST";
			while(res->next()){
				p = p + "|" + res->getString(1) + "|" + res->getString(2) + "|" + res->getString(3);
			}
			delete t;
			return p;
		}
		delete t;
	
	}

	if(token[0] == "POST_MY_IP"){
		std::string e,s = "UPDATE auth SET ip = \""+token[2]+"\",port=\""+token[3]+"\" WHERE username=";
		s += "\""+USERNAME+"\"";
		std::cout << s;
		int res = runUpdate(s,t,e);
		if(res == -1){
			delete t;
			return "POST_MY_IP|" + e;
		}
		
		return "POST_MY_IP_SUCCESS";
	} 

	std::cout << token[0] << std::endl	;
	return "";
	delete t;
}


void logOut(){
	struct sql_conn *t = new sql_conn();
	std::string e,s = "UPDATE auth SET loggedIn = 0,ip=\"\",port=\"\" WHERE username =\""+USERNAME+"\"";
	int res = runUpdate(s,t,e);
	delete t;
}

void setLoggedIn(){
	struct sql_conn *t = new sql_conn();
	std::string e,s = "UPDATE auth SET loggedIn = 1,ip=\"\",port=\"\" WHERE username =\""+USERNAME+"\"";
	int res = runUpdate(s,t,e);
	delete t;
}