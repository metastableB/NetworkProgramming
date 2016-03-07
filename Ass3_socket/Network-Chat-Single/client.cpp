#include "client.h"

Client::Client()
{

}

void Client::newConnection(QTcpSocket *t){
    socket = t;
    /* stop qtcp srver listen */
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readReady()));
}

void Client::disconnected()
{
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
    emit clientMessage("<font color=green>Connected</font>");
}
