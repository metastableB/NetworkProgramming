#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(Authentication* auth,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    authServ = auth;
    isLoggedIn =  true;
    postMyIpSuccess = false;
    getFriendListSuccess = false;
}
void ChatWindow::chatInit(){
    handleMessage("<font color=green>Successfully authenticated</font>");
    handleMessage("<font color=blue>Initializing Chat</font>");
    client = new Client();
    server = new Server(client,this);
    myNickName = authServ->getUsername();
    QObject::connect(authServ, SIGNAL(authenticationMessage(QString)),
                                       this, SLOT(authenticationMessage(QString)));
    QObject::connect(this->server, SIGNAL(newConnection(QTcpSocket*)),
                     this->client, SLOT(newConnection(QTcpSocket*)));
    QObject::connect(this->client, SIGNAL(newConnectionClient(QTcpSocket*)),
                     this->client, SLOT(newConnection(QTcpSocket*)));
    QObject::connect(this,SIGNAL(sendMessage(QString)),client,
                     SLOT(sendMessage(QString)));
    QObject::connect(client,SIGNAL(clientMessage(QString)),
                     this,SLOT(handleMessage(QString)));
    QObject::connect(this->ui->menuFile,SIGNAL(triggered(QAction*)),
                     this,SLOT(slotOptionsMenu(QAction*)),Qt::UniqueConnection);

    handleMessage("<font color=blue>Successfully Initialized</font>");
    handleMessage("<font color=blue>Registering with AuthServer</font>");
    authServ->p_postMyIp(server->serverAddress().toString(),server->serverPort());

    //authServ->getFriendList(server->serverAddress().toString(),server->serverPort());
    /* Should we pause the connection ?
     * What to do with incoming connections?
     *
     * Do not allow to connect to other friends while already connected
     * add a boolean to do keep track of this connection information
     * when someone attempts to connect to another when already connected,
     * ask him to close the existing connection.
     *
     * Add friend can be implemented as drop down option. Pop out a new window to allow
     * adding a  friend. Identify friends with username and IP. (port is random)
     * Send this to the server by establishing a temporary connection.
     * For this we need some kind of protocol and a process managed version of the server
     * Implement that.
     *
     * You will need to figure out the data structre now at the server end as you only send back
     * information about logged in users and not all.
     * Also from the clients end, ping the server every few seconds to get the latest logged in
     * user statistics. (Infact do we need to close the TCP connection ? )
     */
}

    ChatWindow::~ChatWindow()
{
    delete ui;
}

bool ChatWindow::getLineEditMessage(){
    QString s = this->ui->messageEdit->text();
    if(s.length() == 0) return true;
    qDebug() << "Got " << s;
    if (s.startsWith(QChar('/'))) {
        QStringList l = s.split(QChar(' '));
        QString msg = "<font color=red>Invalid Usage </font>";
        this->ui->messageEcho->append(msg);
        this->ui->messageEdit->clear();

    }else if( client->getIsConnected()){
        qDebug() << "Seemes like we are connected";
        QString uname = "<font color=red>" +myNickName+": "+ s + "</font>";
        this->ui->messageEcho->append(uname);
        this->ui->messageEdit->clear();
        emit sendMessage(myNickName + ":" + s);
    }else {
        QString uname = "<font color=orange>You are not connected</font>";
        this->ui->messageEcho->append(uname);
        this->ui->messageEdit->clear();
    }
    return true;
}

void ChatWindow::createFriendsList(QString l){
    /* We get a username|IP|port */
    this->ui->friendsList->clear();
    QStringList friendsList = l.split('|');
    for(int i = 0; i < friendsList.length(); i+=3){
        qDebug() << i ;
        if(friendsList.at(i) == "") break;
        friends[friendsList.at(i)] = std::pair<QString,QString>(friendsList.at(i+1),friendsList.at(i+2));
        this->ui->friendsList->addItem(friendsList.at(i));
    }

}

bool ChatWindow::friendSelected(QModelIndex){
    QString s = this->ui->friendsList->currentItem()->text();

    if(client->getIsConnected())
        this->ui->messageEcho->append("<font colog = organe>You are already connected. Disconnect first</font>");
    else if(friends[s].first == "" || friends[s].second == "")
        this->ui->messageEcho->append("<font color=orange>"+s +" is not logged in</font>");
    else {
        this->ui->messageEcho->append("<font color=blue>Connecting to "+
                                  friends[s].first+":"+friends[s].second + "</font>");
        client->connectTo(friends[s].first, friends[s].second);
    }
    return true;
}

void ChatWindow::authenticationMessage(QString msg){
    qDebug() << "Received msg" << msg;
    if(msg.compare("DISCONNECTED") == 0)
        this->ui->messageEcho->append("<font color=red>" + msg + "</font>");

    else if(msg.compare("CONNECTIONERROR") == 0)
        this->ui->messageEcho->append("<font color=red>ERROR: Connection Error</font>");

    else if(msg.compare("AUTHENTICATED") == 0){
        isLoggedIn = true;
        this->ui->messageEcho->append("<font color=green>Successfully authenticated.</font>");
        chatInit();
    }

    else if(msg.contains("FRIENDLIST")){
        QStringList list = msg.split('|');
        QString l = msg.mid(QString("FRIENDLIST|").length());
        qDebug() << "Creating list with " << l;
        createFriendsList(l);
        handleMessage("<font color=blue>Friend List retrived</font>");
    }

    else if(msg.compare("POST_MY_IP_SUCCESS") == 0){
        postMyIpSuccess = true;
        handleMessage("<font color=blue>Registration successful</font>");
        handleMessage("<font color=blue>Retriving friend list</font>");
        authServ->p_getFriendList();
    }
    else if(msg.compare("POST_MY_IP_FAILURE") == 0){
        postMyIpSuccess = false;
        handleMessage("<font color=red>IP registration failed</font>");
        handleMessage("<font color=red>Exiting..</font>");
    } else this->ui->messageEcho->append("<font color=green>"+msg+"</font>");
}

void ChatWindow::handleMessage(QString s){
    this->ui->messageEcho->append(s);
}

void ChatWindow::slotOptionsMenu(QAction* a){
    qDebug() << "Add Freidn fired" << a->text();
    if(a->text() == "Add Friend"){
        qDebug() << "ADD FIREND";
        AddFriendDialog* a = new AddFriendDialog();
        QObject::connect(a,SIGNAL(okClicked(QString)),
                         this,SLOT(addFriendHandler(QString)));
        a->show();
    }else if(a->text() == "Refresh Friend List"){
        authServ->p_getFriendList();
    }else if(a->text() == "Leave Chat"){
       if(client->getIsConnected()){
           qDebug() << "We should be disconnected by now\n";
           client->close();
       }

    }else if(a->text() == "Exit")
        exit(0);
}

void ChatWindow::addFriendHandler(QString u){
    qDebug() << u;
    authServ->p_addFriend(u);
}
