

#include <QtNetwork>
#include "server.h"

Server::Server(Client* c,QObject *parent)
    : QTcpServer(parent)
{
    client = c;
    listen(QHostAddress::LocalHost);
    qDebug() << "Listening on " << serverAddress() << ":" << serverPort();
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *t = new QTcpSocket(this);
    t->setSocketDescriptor(socketDescriptor);
    qDebug() << "We have a new connection" << t->peerAddress();
    if(client->getIsConnected() ){
        t->close();
        delete t;
    } else
        emit newConnection(t);
}
