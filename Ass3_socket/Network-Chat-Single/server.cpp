

#include <QtNetwork>
#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{

    listen(QHostAddress::LocalHost);
    qDebug() << "Listening on " << serverAddress() << ":" << serverPort();
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *t = new QTcpSocket(this);
    t->setSocketDescriptor(socketDescriptor);
    qDebug() << "We have a new connection" << t->peerAddress();
    emit newConnection(t);
}
