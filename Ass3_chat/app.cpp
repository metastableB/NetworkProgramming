#include "definitions.h"

sem_t sem_server;
sem_t sem_driver;
sem_t sem_client;
struct server_ srv_;
struct client_ cli_;

pthread_t pt_server;
pthread_t pt_client;

int bound_port;

void *client_wait(void* i){
	sleep(1);
	std::cout << "Waiting" << std::endl;
	struct server_* srv = (struct server_ *)i;
	int portno;
	std::string host;
	std::string user_id =  srv->user_id;
	while(srv->status == 0){
		std::cin >> portno >> host;
		if(srv->status != 0) break;
		srv->status = 3;
		client_thread(portno,host,user_id,5);
		break;
	}
	pthread_exit(&srv->status);
	return nullptr;
}

void server_thread(int portno,std::string user_id){
	srv_.portno = portno;
	srv_.status = -1;
	srv_.user_id = user_id;
	srv_.sem_server = &sem_server;
	srv_.sem_driver = &sem_driver;
	sem_init(&sem_server,0,0);
	sem_init(&sem_driver,0,0);
	pthread_create(&pt_server,NULL,server,(void *)&srv_);
	sem_wait(&sem_driver);
	if(srv_.status != 0){
		std::cout << "Status " << srv_.status ;
		error("\nServer creation failed.\n");
		exit(1);
	}
}

void client_thread(int portno,std::string hostname, std::string user_id,int status){
	cli_.portno = portno;
	cli_.hostname = hostname;
	cli_.user_id = user_id;
	cli_.sem_driver = &sem_driver;
	cli_.sem_client = &sem_client;
	cli_.status = -1;
	cli_.bound_port = bound_port;
	if(status == 5)
		cli_.status_con = 5;
	else cli_.status_con = 0;
	sem_init(&sem_client,0,0);
	pthread_create(&pt_client,NULL,client,(void *)&cli_);
	sem_wait(&sem_driver);

	if(cli_.status != 0){
		std::cout << "Status " << srv_.status << std::endl ;
		error("Client creation failed.\n");
		exit(1);
	}
}

int main(int argc, const char* argv[]){
	if(argc < 2){
		error("Usage :\nserver_port user_id\n");
		return 0;
	}
	std::string user_id = argv[2];
	server_thread(atoi(argv[1]), user_id);
	bound_port = atoi(argv[1]);

	sem_post(&sem_server);
	
	pthread_t c_wait;
	pthread_create(&c_wait,NULL,client_wait,(void *)&srv_);
	sem_post(&sem_client);
	pthread_join(pt_server,nullptr);
	return 0;
}