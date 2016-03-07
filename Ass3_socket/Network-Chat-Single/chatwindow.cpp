#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    isLoggedIn =  false;
    QObject::connect(&authServ, SIGNAL(authenticationMessage(QString)),
                                       this, SLOT(authenticationMessage(QString)));
    //authenticationHelper();
    myNickName = "metastableB_GUI";

    isLoggedIn = true;
    chatInit();
}

void ChatWindow::authenticationHelper(){
    this->ui->messageEcho->append("User /login <username> <password> to authenticate");
}

void ChatWindow::chatInit(){
    server = new Server(this);
    client = new Client();
    QObject::connect(this->server, SIGNAL(newConnection(QTcpSocket*)),
                     this->client, SLOT(newConnection(QTcpSocket*)));
    QObject::connect(this->client, SIGNAL(newConnectionClient(QTcpSocket*)),
                     this->client, SLOT(newConnection(QTcpSocket*)));
    QObject::connect(this,SIGNAL(sendMessage(QString)),client,
                     SLOT(sendMessage(QString)));
    QObject::connect(client,SIGNAL(clientMessage(QString)),
                     this,SLOT(handleMessage(QString)));
    QString l = "metastableB|127.0.0.1|3500|nemesis|127.0.0.1|40764";
    createFriendsList(l);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

bool ChatWindow::getLineEditMessage(){
    QString s = this->ui->messageEdit->text();
    if(s.length() == 0) return true;
    if (s.startsWith(QChar('/'))) {
          QStringList l = s.split(QChar(' '));
            if(l[0].compare("/login") == 0 && !isLoggedIn){
                if(l.size() != 3) {
                    QString msg = "<font color=red>Invalid Usage </font>";
                    this->ui->messageEcho->append(msg);
                    this->ui->messageEdit->clear();
                }
                myNickName = l.at(1);
                this->ui->messageEcho->append(tr("Authenticating: %1")
                    .arg(myNickName));

                if(!isLoggedIn){
                   authServ.setCredentials(l.at(1),l.at(2));
                   authServ.authenticate();
                }
            } else{
                QString msg = "<font color=red>Invalid Usage </font>";
                this->ui->messageEcho->append(msg);
                this->ui->messageEdit->clear();
            }

    }else{
        QString uname = "<font color=red>" +myNickName+": "+ s + "</font>";
        this->ui->messageEcho->append(uname);
        this->ui->messageEdit->clear();
        emit sendMessage(myNickName + ":" + s);
    }
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
    this->ui->messageEcho->append("<font color=blue>Connecting to "+
                                  friends[s].first+":"+friends[s].second + "</font>");
    client->connectTo(friends[s].first, friends[s].second);
    return true;
}

void ChatWindow::authenticationMessage(QString msg){
    qDebug() << "Received msg" << msg;
    // if teh message is good: we proceede clientInit();
    if(msg.compare("DISCONNECTED") == 0)
        this->ui->messageEcho->append("<font color=red>" + msg + "</font>");

    else if(msg.compare("CONNECTIONERROR") == 0)
        this->ui->messageEcho->append("<font color=red>ERROR: Connection Error</font>");

    else if(msg.compare("AUTHENTICATED") == 0){
        isLoggedIn = true;
        authServ.close();
        this->ui->messageEcho->append("<font color=green>Successfully authenticated.</font>");
        chatInit();
    } else
        this->ui->messageEcho->append("<font color=green>"+msg+"</font>");
}

void ChatWindow::handleMessage(QString s){
    this->ui->messageEcho->append(s);
}
