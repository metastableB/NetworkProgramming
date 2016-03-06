#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    this->ui->messageEcho->append("Starting Server..");
    QString l = "metastableB|192.165.189.120|8090|nemesis|172.168.1.1|5500";
    createFriendsList(l);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

bool ChatWindow::getLineEditMessage(){
    QString s = this->ui->messageEdit->text();
    if(s.length() == 0) return true;
    QString uname = "<font color=red>metastableB </font>";
    s = uname + s;
    this->ui->messageEcho->append(s);
    this->ui->messageEdit->clear();
    return true;
}

void ChatWindow::createFriendsList(QString l){
    /* We get a username|IP|port */
    QStringList friendsList = l.split('|');
    for(int i = 0; i < friendsList.size(); i+=3){
        friends[friendsList.at(i)] = std::pair<QString,QString>(friendsList.at(i+1),friendsList.at(i+2));
        this->ui->friendsList->addItem(friendsList.at(i));
    }

}

bool ChatWindow::friendSelected(QModelIndex){
    QString s = this->ui->friendsList->currentItem()->text();
    this->ui->messageEcho->append(friends[s].first+":"+friends[s].second);
    return true;
}
//----------------------------

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

void * ChatWindow::server(){
    int sockfd, new_fd;
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

    if ((rv = getaddrinfo(NULL, this->bindport, &hints, &servinfo)) != 0) {
        this->ui->messageEcho->append("ERROR: getaddrinf");
        exit(1);
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            this->ui->messageEcho->append("ERROR: server:socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            this->ui->messageEcho->append("ERROR: setsockopt");
            exit(1);
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            ::close(sockfd);
            this->ui->messageEcho->append("ERROR: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo); // all done with ths structure

    if (p == NULL)  {
        this->ui->messageEcho->append("ERROR: failed to bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        this->ui->messageEcho->append("ERROR: listen");
        exit(1);
    }
    /* MAGIC CODE : No idea how ths works */
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        this->ui->messageEcho->append("ERROR: sigaction");
        exit(1);
    }

    //this->ui->messageEcho->append("Waiting for connections");

    char buf[MAXDATASIZE];
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
        this->ui->messageEcho->append("ERROR: accept");
        exit(1);
    }

    inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr *)&their_addr),
        s, sizeof s);
    this->ui->messageEcho->append("Connected to" + QString(s));

    ::close(sockfd); // We dont listen anymore

    /* Server only reads from socket and prints to screen
     * The main thread reads from standard input and prints to socket
     */

    while(1){
        int bread;
        bread = recv(new_fd, buf, MAXDATASIZE - 1, 0);
        if(bread <= 0){
            this->ui->messageEcho->append("ERROR: recv");
            exit(1);
        }
        buf[bread] = '\0';
        this->ui->messageEcho->append(buf);
    }
    ::close(new_fd);
}



