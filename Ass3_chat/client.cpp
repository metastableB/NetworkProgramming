/* Refer server file for more documentaion */

#include "definitions.h"

struct client_rw {
    bool kill_thread;
    int sockfd;
    struct client_ *cli;
} c_rw;

void* pt_select_rw_c(void* i){
    struct client_rw *rw = (struct client_rw*) i ;
    struct client_ *cli =  rw->cli;
    char buffer[256];
    int n;

    std::queue<std::string> inq,outq;

    struct timeval tv;
    fd_set readfds, writefds;
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
            std::cout << "read from stdin" << std::endl;
            std::cin >> std_in;
            if(std_in[0] == '#') {
                std_in = "#";
                outq.push(std_in);
                break;
            }else {
                std_in = cli->user_id + ": " + std_in;
                outq.push(std_in);
            }
        }

        if(FD_ISSET(rw->sockfd, &readfds)){
            std::cout << "read from soc" << std::endl;
            memset(buffer,0,256);
            n = read(rw->sockfd,buffer,255);
            if (n < 0) {
                error("ERROR reading from socket");
                cli->status = 1;
                rw->kill_thread = true;
                pthread_exit(&cli->status);
            }
            if(buffer[0] == '#') break;
            inq.push(std::string(buffer));
        }

        if(FD_ISSET(rw->sockfd,&writefds)){
            std::cout << "write stdin" << std::endl;
            std::cout << "Blocking on write server" << std::endl;
            std::string buff = outq.front();
            outq.pop();
            n = write(rw->sockfd,buff.c_str(),strlen(buff.c_str()));
            if(buff == "#") break;
            if (n < 0) {
                error("ERROR writing to socket");
                cli->status = 1;
                rw->kill_thread = true;
                pthread_exit(&cli->status);
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

void *pt_write_skt_c(void *i){
    std::cout << "write\n";
    struct client_rw *rw = (struct client_rw*) i ;
    struct client_ *cli = rw->cli;
    char buffer[256];
    auto sockfd = rw->sockfd;
    int n;
    while(!rw->kill_thread){
        bzero(buffer,256);
        fgets(buffer,245,stdin);
        std::string buff(buffer);
        if(buffer[0] == '#')
            break;
        buff = cli->user_id + ": " + buff;
        n = write(sockfd,buff.c_str(),strlen(buff.c_str()));
        if (n < 0) {
            error("ERROR writing to socket");
            cli->status = 1;
            rw->kill_thread = true;
            pthread_exit(&cli->status);
        }
    }
    rw->kill_thread = true;
    return nullptr;
}

void *pt_read_skt_c(void* i){
    std::cout << "read\n";
    struct client_rw *rw = (struct client_rw*) i;
    struct client_ *cli =  rw->cli;
    char buffer[256];
    int n;
    auto sockfd = rw->sockfd;
    while(!rw->kill_thread){
        memset(buffer,0,256);
        n = read(sockfd,buffer,255);

        if (n < 0) {
            error("ERROR reading from socket");
            cli->status = 1;
            rw->kill_thread = true;
            pthread_exit(&cli->status);
        }
        if(buffer[0] == '#') break;
        printf("%s",buffer);
    }
    rw->kill_thread = true;
    return nullptr;
}

void *client(void *i)
{
    struct client_ *cli = (struct client_*) i;
    std::string hostname = cli->hostname;
    int portno = cli->portno;
    sem_t *sem_client = cli->sem_client;
    sem_t *sem_driver = cli->sem_driver;

    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
        cli->status = 1;
        sem_post(sem_driver);
        pthread_exit(&cli->status);
    }
    server = gethostbyname(hostname.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        cli->status = 1;
        sem_post(sem_driver);
        pthread_exit(&cli->status);
    }
    
    memset((char *) &serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR connecting");
        cli->status = 1;
        sem_post(sem_driver);
        pthread_exit(&cli->status);
    } 
    cli->status = 0;
    sem_post(sem_driver);
    //sem_wait(sem_client)
    c_rw.kill_thread = false;
    c_rw.sockfd = sockfd;
    c_rw.cli = cli;

    /*pthread_t pt_r,pt_w;
    pthread_create(&pt_r,NULL,pt_read_skt_c,(void *)&c_rw);
    pthread_join(pt_r,nullptr);
    pthread_create(&pt_w,NULL,pt_write_skt_c,(void *)&c_rw);
    pthread_create(&pt_r,NULL,pt_select_rw_c,(void*)&c_rw);
    pthread_join(pt_r,nullptr);*/
    char buffer[256];
    std::string buff;
    int n;
    if(cli->status_con == 5){
        std::cout << "Sending connection info\n";
        buff = std::to_string(cli->bound_port) + " " + hostname;
        n = write(sockfd,buff.c_str(),strlen(buff.c_str()));
    }

    if (n < 0) {
        error("ERROR writing to socket");
        cli->status = 1;
        pthread_exit(&cli->status);
    }
    printf("Begin chat\n");
    while(true){
        buff = "";
        char * c = nullptr;
        while(c == nullptr)
            c = fgets(buffer,255,stdin);
        std::cout << "You: " << buffer ;
        buff = buffer;
        if(buff[0] == '#') {
            buff ="#";
            n = write(sockfd,buff.c_str(),strlen(buff.c_str()));
            if (n < 0) {
                error("ERROR writing to socket");
                cli->status = 1;
                pthread_exit(&cli->status);
            }
        }else if(std::string(buff).length()!=0) {
            buff = cli->user_id+": "+ buff ;
            n = write(sockfd,buff.c_str(),strlen(buff.c_str()));
            if (n < 0) {
                error("ERROR writing to socket");
                cli->status = 1;
                pthread_exit(&cli->status);
            }
        }else sleep(1);
        if(buff =="#") break;
   }
    close(sockfd);
    std::cout << "Exiting chat\n";
    return 0;
}
