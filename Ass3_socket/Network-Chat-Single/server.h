#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);

signals:
    void newConnection(QTcpSocket *connection);

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
};

#endif
