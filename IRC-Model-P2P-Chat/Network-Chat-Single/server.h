#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "client.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(Client* c,QObject *parent = 0);
    Client* client;

signals:
    void newConnection(QTcpSocket *connection);

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
};

#endif
