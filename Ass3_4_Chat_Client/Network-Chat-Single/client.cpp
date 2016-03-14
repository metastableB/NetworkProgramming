#include "client.h"

Client::Client()
{
    isConnected = false;
    socket = NULL;
    allowGroupChat = false;
}

void Client::newConnection(QTcpSocket *t){
    socket = t;
    isConnected = true;
    /* stop qtcp srver listen */
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readReady()));
    if(getGroupChatAllowed())
        peers.insert(socket->peerPort(),socket);
}

void Client::disconnected()
{
    qDebug() << "Disconnected\n";
    if(!allowGroupChat){
        isConnected = false;
        socket = NULL;
        emit clientMessage("Disconnected");
    }
    else {
        QTcpSocket* t = qobject_cast<QTcpSocket*>(sender());
        peers.remove(t->peerPort());
        emit clientMessage("Disconnected " + t->peerAddress().toString() + ":" + QString(QString::number(t->peerPort())));
    }
}

void Client::connectionError(QAbstractSocket::SocketError /* socketError */)
{
    emit clientMessage("Connection Error");
}

void Client:: readReady(){
    QByteArray data;
    data.resize(256);
    if(!allowGroupChat)
        data = socket->readAll();
    else {
        QTcpSocket* t = qobject_cast<QTcpSocket*>(sender());
        data = t->readAll();
        sendMessage(data);
    }
    qDebug() << "Received " << data;
    QString msg(data);
    msg = "<font color=purple>" +msg+"</font>";
    emit clientMessage((msg));
}

void Client::sendMessage(QString msg){
    qDebug() << "Sending to socket";
    QString data(msg);
    if(!allowGroupChat)
        socket->write(data.toLatin1().data(),data.length());
    else{
        QList <QTcpSocket*> p = peers.values();
        foreach (QTcpSocket *t, p)
                t->write(data.toLatin1().data(),data.length());
    }
}

void Client::connectTo(QString ip, QString port){
    socket = new QTcpSocket(this);
    qDebug() << "Attempt to connect";
    socket->connectToHost(ip,port.toInt());
    emit newConnectionClient(socket);
}

void Client::connected(){
    isConnected = true;
    emit clientMessage("<font color=green>Connected</font>");
}

bool Client::getIsConnected(){
    return isConnected;
}


bool Client::getGroupChatAllowed(){
    return allowGroupChat;
}

void Client::close(){
    delete socket;
    socket = NULL;
}

/* Group Chat */
void Client::initGroupChat(Authentication* t,QString s, quint16 su){
    auth = t;
    s_ip = s;
    s_port = su;
}

void Client::joinGroup(QString grp){
    if(isConnected){
        emit clientMessage("Something is wrong in the code in group chat");
        return;
    }
    auth->p_joinGroup(grp);

}

void Client::createGroup(QString grp){
    if(isConnected){
        emit clientMessage("Something is wrong in the code in group chat");
        return;
    }
    allowGroupChat = true;
    qDebug() << "Allow group chat is true";
    auth->p_createGroup(grp,s_ip,s_port);
}

void Client::joinGroupChat(QString ip , QString port){
    connectTo(ip,port);
    qDebug() << "Here is somethign";
}
