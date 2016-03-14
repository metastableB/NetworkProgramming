

#include <QtNetwork>

#include "connection.h"
#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    /* Listens to any incoming connection
     * incommingConnection opens a new socket and
     * emits newConnection with the connection info
     *
	 * Currently listening on all network interfaces
	 * we need to specify a port and a connection later
	 * Port is chosen automatically
	 */
    listen(QHostAddress::LocalHost);
    qDebug() << "Listening on " << serverAddress() << ":" << serverPort();
    qDebug() << "We are listening asynchronously";
}

/*  Called by QTcpServer on new incoming connection 
	Connection is a wrapper class over a QTcpSocket
	used to implement some custom methods for reading
	and writing and some protocol definition 
	The Connection object is used to establish a connection.
	Lets assume that connection->setSocketDescriptor is causing
	the connetion attempt and emitting the connected disconnected
	signal.*/

void Server::incomingConnection(qintptr socketDescriptor)
{
    Connection *connection = new Connection(this);
    connection->setSocketDescriptor(socketDescriptor);
    qDebug() << "We have a new connection" << connection->peerAddress();
    emit newConnection(connection);
}
