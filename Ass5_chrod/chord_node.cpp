#include "chord_node.h"

/*
 * PROTOCOL DEFINITIONS
 *
 * << GET_CPF|k
 * >> POST_CPF|<cpf_ip>|<cpf_port>|<cpf_key>
 *
 * << UPDATE_PREDECESSOR|nodeip|port|key
 * >> UPDATED_PREDECESSOR
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
 *
 * >> FIND_PREDECESSOR|key
 * << FOUND_PREDECESSOR|ip|port|key
 *
 * >> ADD_DATA|data
 * << ADDED_DATA
 */
Chord_Node::Chord_Node(std::string ip, int port, int key){
	set_ip(ip);
	set_port(port);
	set_key(key);
	predecessor = node_info("",-1,-1);
}

void Chord_Node::join(std::string i, int p){
	if( i == "" || p < 0){
		for(int i = 1; i  <= M_M; i++)
			finger_table[i] = std::pair<int,node_info>(start(i),node_info(this->ip,
				this->port,this->key));
		predecessor = node_info(this->ip,this->port,this->key);
	} else {
		init_finger_table(i,p);
		update_others();
		migrate_data(predecessor);
		migrate_data(finger_table[1].second);
	}
	std::cout << "\n\n*****************";
	std::cout << "\njoin complete \n";
	std::cout << "*****************\n\n";
}

void Chord_Node::init_finger_table(std::string i, int p) {

	finger_table[1] = std::pair<int,node_info>(start(1),n_find_successor(start(1),i,p));
	int x;
	predecessor = n_find_predecessor(finger_table[1].second.key,finger_table[1].second.ip, finger_table[1].second.port);
	update_predecessor(finger_table[1].second); // Update predecessor entry of successor
	for(int j = 1; j < M_M; j++){
		if(belongs_i_e(start(j+1), 
			this->key,finger_table[j].second.key))
			finger_table[j+1] = std::pair<int,node_info>(start(j+1),finger_table[j].second);
		else {
			finger_table[j+1] = std::pair<int,node_info>(start(j+1),n_find_successor(start(j+1),i ,p));
		}
	}

}
/*
 * Update fingertable of all other nodes
 * which should have us as the entry
 */

void Chord_Node::update_others(){
	/*
	 * >> UPDATE_FINGER_TABLE|ip|port|key|i
	 * << UPDATED_FINGER_TABLE
	 */
	node_info p;
	for(int i = 1; i <= M_M; i++){
		int key = this->key - pow(2,i-1);
		if(key < 0) key = POW_M - ((-1*key)%POW_M);
		p = n_find_predecessor(key);
		std::string msg = "UPDATE_FINGER_TABLE";
		msg += "|" + this->ip + "|" + std::to_string(this->port) + "|" + std::to_string(this->key);
		msg += "|" + std::to_string(i);
		int sockfd = get_socket_fd(p.ip,p.port);
		if(sockfd < 0) {printf("ERROR UPDATE OTHERS"); exit(2);}
		std::string r = proto_exec(msg,sockfd);
		close(sockfd);
#ifdef __DEBUG__
		std::cout << this->key << " << " << msg << std::endl;
		std::cout << this->key << " >> " << r << std::endl;
#endif
		if(r != "UPDATED_FINGER_TABLE") {
			std::cout << "Incorrect response to UPDATED_FINGER_TABLE : " << r << std::endl ;
			exit(2);
		}
	}
}
/*
 * If k is the ithe finger of n, update 
 */
void Chord_Node::update_finger_table(node_info new_f, int i){
	if(belongs_i_e(new_f.key, this->key, finger_table[i].second.key)){
		finger_table[i].second = new_f;
		finger_table[i].first = start(i);
		auto p = predecessor;
		std::string msg = "UPDATE_FINGER_TABLE";
		msg += "|" + this->ip + "|" + std::to_string(this->port) + "|" + std::to_string(this->key);
		msg += "|" + std::to_string(i);
		int sockfd = get_socket_fd(p.ip,p.port);
		if(sockfd < 0) {printf("ERROR UPDATE FINGERTABLE"); exit(2);}
		std::string r = proto_exec(msg,sockfd);
		close(sockfd);
#ifdef __DEBUG__
		std::cout << this->key << " << " << msg << std::endl;
		std::cout << this->key << " >> " << r << std::endl;
#endif
		if(r != "UPDATED_FINGER_TABLE") {
			std::cout << "Incorrect response to UPDATED_FINGER_TABLE : " << r << std::endl ;
			exit(2);
		}
	}
}

/* If there is a node with k,
 * this should return k
 */
struct node_info Chord_Node::n_find_successor(int k){
	if(this->key == k) return node_info(this->ip,this->port,this->key);
	struct node_info n = n_find_predecessor(k);
	return get_finger_1(n.ip,n.port);
}

struct node_info Chord_Node::n_find_successor(int k, std::string i, int p){
	/*
	 * >> FIND_SUCCESSOR|key
 	 * << FOUND_SUCCESSOR|ip|port|key
 	 */
	std::string msg = "FIND_SUCCESSOR";
	msg += "|" + std::to_string(k);

	int sockfd = get_socket_fd(i,p);
	if(sockfd < 0) return node_info("",-1,-1);
	std::string r = proto_exec(msg,sockfd);
	close(sockfd);
#ifdef __DEBUG__
	std::cout << this->key << " << " << msg << std::endl;
	std::cout << this->key << " >> " << r << std::endl;
#endif
	std::vector<std::string> tokens;
	tokenize(r,tokens);
	if(tokens[0] != "FOUND_SUCCESSOR") {
		std::cout << "Incorrect response to FIND_SUCCESSOR : " << tokens[0] << std::endl ;
		return node_info("",-1,-1);
	}
	return node_info(tokens[1],std::stoi(tokens[2]),std::stoi(tokens[3]));
}
struct node_info Chord_Node::n_find_predecessor(int k){
	struct node_info n,s;
	n.ip = ip;
	n.port = port;
	n.key = key;
	s = finger_table[1].second;
	int ssr = s.key;
	if(n.key == s.key) return n;
	while(!belongs_e_i(k,n.key,ssr)){
		n = n_cpf(k, n.ip, n.port);
		if(n.key < 0){
			printf("n_cpf error: %s %d\n",n.ip.c_str(),n.port);
			exit(1);
		}
		s = get_finger_1(n.ip,n.port);
		if(s.key < 0){
			printf("n_find_successor error: %s %d\n", n.ip.c_str(), n.port);
			exit(2);
		}
		ssr = s.key;
		if(n.key == s.key) break;
	}
	return n;
}

struct node_info Chord_Node::n_find_predecessor(int k, std::string i, int p){
	/*
	 * >> FIND_PREDECESSOR|key
 	 * << FOUND_PREDECESSOR|ip|port|key
 	 */
	std::string msg = "FIND_PREDECESSOR";
	msg += "|" + std::to_string(k);

	int sockfd = get_socket_fd(i,p);
	if(sockfd < 0) return node_info("",-1,-1);
	std::string r = proto_exec(msg,sockfd);
	close(sockfd);
#ifdef __DEBUG__
	std::cout << this->key << " << " << msg << std::endl;
	std::cout << this->key << " >> " << r << std::endl;
#endif
	std::vector<std::string> tokens;
	tokenize(r,tokens);
	if(tokens[0] != "FOUND_PREDECESSOR") {
		std::cout << "Incorrect response to FIND_PREDECESSOR : " << tokens[0] << std::endl ;
		return node_info("",-1,-1);
	}
	return node_info(tokens[1],std::stoi(tokens[2]),std::stoi(tokens[3]));
}


struct node_info Chord_Node::n_cpf(int k,std::string i, int p){
	/*
	 *  << GET_CPF|k
 	 *  >> POST_CPF|<cpf_ip>|<cpf_port>|<cpf_key>
 	 */
	if(i == this->ip && p == this->port) return get_cpf(k);
	std::string msg = "GET_CPF";
	msg += "|" + std::to_string(k);

	int sockfd = get_socket_fd(i,p);
	if(sockfd < 0) return node_info("",-1,-1);
	std::string r = proto_exec(msg,sockfd);
	close(sockfd);
#ifdef __DEBUG__
	std::cout << this->key << " << " << msg << std::endl;
	std::cout << this->key << " >> " << r << std::endl;
#endif
	std::vector<std::string> tokens;
	tokenize(r,tokens);
	if(tokens[0] != "POST_CPF") {
		std::cout << "Incorrect response to GET_CPF : " << tokens[0] << std::endl ;
		return node_info("",-1,-1);
	}
	return node_info(tokens[1],std::stoi(tokens[2]),std::stoi(tokens[3]));
}

//***************************
// PRIVATE CALLS by NETWORK
//***************************
struct node_info Chord_Node::get_cpf(int k){
	for(int i = M_M; i >= 1; i--)
		if(belongs_e_e(finger_table[i].second.key,key,k))
			return finger_table[i].second	;
	return node_info(this->ip, this->port, this->key);	
}

/*
 * This returns the true successor
 * i.e. the node following the given node
 */
struct node_info Chord_Node::get_finger_1(std::string i, int p){
/*
 * << GET_FINGER_1
 * >> POST_FINGER_1|<FINGER_1_ip>|<FINGER_1_port>|<FINGER_1_key>
 */

 	if(i == this->ip && p == this->port) return finger_table[1].second;
	std::string msg = "GET_FINGER_1";

	int sockfd = get_socket_fd(i,p);
	if(sockfd < 0) return node_info("",-1,-1);
	std::string r = proto_exec(msg,sockfd);
	close(sockfd);
#ifdef __DEBUG__
	std::cout << this->key << " << " << msg << std::endl;
	std::cout << this->key << " >> " << r << std::endl;
#endif
	std::vector<std::string> tokens;
	tokenize(r,tokens);
	if(tokens[0] != "POST_FINGER_1") {
		std::cout << "Incorrect response to GET_FINGER_1 : " << tokens[0] << std::endl ;
		return node_info("",-1,-1);
	}
	return node_info(tokens[1],std::stoi(tokens[2]),std::stoi(tokens[3]));
}

/*
 * this node just joined. We wan't to update the
 * successors predecesor entry. Successor being
 * finger[1];
 */
void Chord_Node::update_predecessor(node_info& s){
	/*
	 * UPDATE_PREDECESSOR|nodeip|port|key
	 * UPDATED_PREDECESSOR
	 */
	std::string msg("UPDATE_PREDECESSOR");
	msg += "|" + this->ip;
	msg += "|"+std::to_string(this->port)+"|"+std::to_string(this->key);

	int sockfd = get_socket_fd(s.ip,s.port);
	if(sockfd < 0) {
		std::cout << "UPDATE PREDECESSOR FAILED";
		exit(1);
	}
	std::string r = proto_exec(msg,sockfd);
	close(sockfd);
#ifdef __DEBUG__
	std::cout << this->key << " << " << msg << std::endl;
	std::cout << this->key << " >> " << r << std::endl;
#endif
	if(r != "UPDATED_PREDECESSOR"){
		std::cout << "PREDECESSOR UPDATE FAILED :" << r<< std::endl;
		exit(1);
	}
}

/*
 * This is joining, update finger[1] of our
 * predecessor
 */

void Chord_Node::update_successor(node_info& s){
	/*
	 * UPDATE_SUCCESSOR|nodeip|port|key
	 * UPDATED_FINGER_1
	 */
	std::string msg = "UPDATE_FINGER_1";
	msg += "|" + this->ip ;
	msg += "|"+std::to_string(this->port)+"|"+std::to_string(this->key);

	int sockfd = get_socket_fd(s.ip,s.port);
	if(sockfd < 0) {
		std::cout << "UPDATE FINGER_1 FAILED";
		exit(1);
	}

	std::string r = proto_exec(msg,sockfd);
#ifdef __DEBUG__
	std::cout << this->key << " << " << msg << std::endl;
	std::cout << this->key << " >> " << r << std::endl;
#endif

	if(r != "UPDATED_FINGER_1"){
		std::cout << "SUCCESSOR UPDATE FAILED " << msg << std::endl;
		exit(1);
	}
}
//*********************
// MAINTANANCE METHODS 
//*********************

void Chord_Node::migrate_data(){
	/* Move all the data of this node around */
	std::vector<int> data2 = data;
	data.clear();
	while(!data2.empty()){
		int a = data2.back();
		data2.pop_back();
		add_data(a);
	}
}

void Chord_Node::migrate_data(node_info p){
	/*
	 * >> MIGRATE_DATA
	 * << MIGRATED
	 */
	
	std::string msg = "MIGRATE_DATA";
	int sockfd = get_socket_fd(p.ip,p.port);
	if(sockfd < 0) {printf("ERROR MIGRATE_DATA"); exit(2);}
	std::string r = proto_exec(msg,sockfd);
#ifdef __DEBUG__
	std::cout << this->key << " << " << msg << std::endl;
	std::cout << this->key << " >> " << r << std::endl;
#endif

	if(r != "MIGRATED") {
		std::cout << "Incorrect response to MIGRATE_DATA : " << r << std::endl ;
		exit(2);
	}
}
void Chord_Node::add_data(int d){
	auto n = n_find_successor(d % POW_M);
	if(n.key == this->key)
		data.push_back(d);
	else {
		/*
		 * >> ADD_DATA|data
	 	 * << ADDED_DATA
	 	 */
		std::string msg = "ADD_DATA";
		msg += "|" + std::to_string(d);

		int sockfd = get_socket_fd(n.ip, n.port);
		if(sockfd < 0) return;
		std::string r = proto_exec(msg,sockfd);
		close(sockfd);
#ifdef __DEBUG__
		std::cout << this->key << " << " << msg << std::endl;
		std::cout << this->key << " >> " << r << std::endl;
#endif
		std::vector<std::string> tokens;
		tokenize(r,tokens);
		if(tokens[0] != "ADDED_DATA") {
			std::cout << "Incorrect response to ADDED_DATA : " << tokens[0] << std::endl ;
			return ;
		}
	}
}

void Chord_Node::set_key(int k){
	key = k;
}

void Chord_Node::set_port(int p){
	port = p;
}

void Chord_Node::set_ip(std::string s){
	ip = s;
}

void Chord_Node::set_predecessor(node_info n){
	predecessor = n;
}
struct node_info Chord_Node::get_finger(int key){
	if(!(key > 0 && key <= M_M)) return node_info("",-1,-1);
	return finger_table[key].second;
}

std::string Chord_Node::get_ip(){
	return ip;
}

int Chord_Node::get_port(){
	return port;
}

int Chord_Node::get_key(){
	return key;
}

std::string Chord_Node::get_data_csv(){
	std::string s;
	std::sort(data.begin(),data.end());
	for(int i: data)
		s = s + std::to_string(i) + ", ";
	return s;
}

struct node_info Chord_Node::get_predecessor(){
	return predecessor;
}

int Chord_Node::start(int k){
	int a = this->key + pow(2,k-1);
	int mod = POW_M;
	a = a % mod;
	return a;
}
bool Chord_Node::belongs_e_e(int a, int l, int u){
	if( u== l) return true;
	if(u >= l) return (a > l && a < u);
	bool r1 = (a > l && a < POW_M);
	bool r2 = (a >= 0 && a < u);
	return r1 || r2;
}

bool Chord_Node::belongs_e_i(int a, int l, int u){
	if(u == l) return true;
	if(u >= l) return (a > l && a <= u);
	bool r1 = (a > l && a < POW_M);
	bool r2 = (a >= 0 && a <= u);
	return r1 || r2;
}

bool Chord_Node::belongs_i_e(int a, int l, int u){
	if(l == u) return true;
	if(u >= l) return (a >= l && a < u);
	bool r1 = (a >= l && a < POW_M);
	bool r2 = (a >= 0 && a < u);
	return r1 || r2;
}
void Chord_Node::tokenize(std::string req, std::vector<std::string> &token){
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

//********************
// Network Methods
//********************
std::string Chord_Node::proto_exec(std::string msg, int sockfd){
	if(send_msg(msg, sockfd) < 0) return "ERROR|COULD_NOT_SEND";
	std::string s = receive_msg(sockfd);
	return s;
}

int Chord_Node::send_msg(std::string msg,int sockfd){
	if (send(sockfd, msg.c_str(), msg.length(), 0) == -1){
		perror("send");	
		return -1;
	}
	return sockfd;
}

std::string Chord_Node::receive_msg(int sockfd){
	int bread;
	char buf[MAXDATASIZE];
	bread = recv(sockfd, buf, MAXDATASIZE - 1, 0);
	if(bread <= 0){
		perror("recv");
		return "ERROR|RECV";
	}
	buf[bread] = '\0';
	return std::string(buf);
}
// get sockaddr, IPv4 or IPv6:
void * Chord_Node::get_in_addr(struct sockaddr *sa){
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int Chord_Node::get_socket_fd(std::string adr, int prt){
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	std::string prt_s = std::to_string(prt);
	if ((rv = getaddrinfo(adr.c_str(), prt_s.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
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
		return -1;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	//printf("client: connecting to %s %d\n", s,prt);
	freeaddrinfo(servinfo); // all done with this structure
	return sockfd;
}


int Chord_Node::test_send(std::string m, std::string i,int p){
	int sockfd = get_socket_fd(i,p);
	close(send_msg(m,sockfd));
}

int Chord_Node::test_proto_exec(std::string m, std::string i,int p){
	int sockfd = get_socket_fd(i,p);
	std::cout << proto_exec(m,sockfd);
	close(sockfd);
}

int Chord_Node::test_random_finger(){
	for(int i = 1; i <= M_M; i++)
		finger_table[i].second = node_info("localhost",3501,key);
}

int Chord_Node::test_print_debug(){
	std::string msg = "\nINFO\n";
	msg += "KEY: " + std::to_string(this->key);
	msg += "\nPREDECESSOR: " + std::to_string(predecessor.key);
	msg += "\n" + this->ip;
	msg += ":" + std::to_string(this->port);
	msg += "\n\n";
	msg += "FINGER TABLE ENTRIES\n";
	msg += "index\tkey\tStart\tIp\t\tPORT\n";
	for(int i = 1; i <= M_M; i++){
		auto &a = finger_table[i].second;
		msg += std::to_string(i)+"\t"+std::to_string(finger_table[i].first);
		msg += "\t" + std::to_string(a.key)+"\t" + a.ip + "\t" + std::to_string(a.port);
		msg += "\n";
	}
	std::cout << msg;
}