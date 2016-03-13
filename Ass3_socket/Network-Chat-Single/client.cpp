#include "client.h"

Client::Client()
{
    isConnected = false;
    socket = NULL;
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
}

void Client::disconnected()
{
    qDebug() << "Disconnected\n";
    isConnected = false;
    socket = NULL;
    emit clientMessage("Disconnected");
}

void Client::connectionError(QAbstractSocket::SocketError /* socketError */)
{
    emit clientMessage("Connection Error");
}

void Client:: readReady(){
    QByteArray data;
    data.resize(256);
    data = socket->readAll();
    qDebug() << "Received " << data;
    QString msg(data);
    msg = "<font color=purple>" +msg+"</font>";
    emit clientMessage((msg));
}

void Client::sendMessage(QString msg){
    qDebug() << "Sending to socket";
    QString data(msg);
    socket->write(data.toLatin1().data(),data.length());
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

void Client::close(){
    delete socket;
    socket = NULL;
}
