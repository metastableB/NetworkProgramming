#include <QtNetwork>

#include "client.h"
#include "connection.h"
#include "peermanager.h"

/* The client object sets up the server (auto call to constructor)
 * then sets up the peermanager by adding its self to the peer.
 * after that it connects the Server->newConnection to 
 * the new Connection slot. This slot sends a greeting message
 * to the new connection on receiving the new connection and
 * when we get a ready conformation, we add it to our peerlist
 * 
 * The server connection is a QTcpSocket wrapper class called 
 * Connection. The server on receiving a connection request, 
 * creates a connection object and passes it to newConnectin 
 * of the client class. The connection objest is instructed 
 * to send a greeting message once a connected() signal is 
 * received from that TCP client. Seems like newConnection 
 * signal can be emitted from two ways, from server
 * as well as peer manager. The action is same in either case.
 *
 * ---------------------
 * Servers Point of View
 * ---------------------
 * From the server's point of view this is what is happening.
 * Upon a new connection attempt, we create a new Connection 
 * and signal newConnection(). newConnection() sets the 
 * Greeting message as our username. The greeting message is 
 * some sort of protocol and the other party reads this.
 * Now we have set the greeting message. Once the connection 
 * is established, connected() is signaled by the connection 
 * object and we send the greeting message from out end.
 * We also expect a greeting message from the other party. 
 * When readReady is signalled we use processReadReady to 
 * check if the protocol is followed. We extract the 
 * username of the peer connected to us and signal readyForUse()
 * In ready for use, we connect the new connection's newMessage 
 * to our handler of new messages.
 * Further we use the connections peer information(from socketTcp) 
 * to add it to our peers list. We conclude by emitting a new 
 * participant which is caught in UI and does some UI work.
 * 
 * ------------------------
 * Clients point of view
 * ------------------------
 * Hypothesis one:
 * The peerManager object opens a broadcasting session using a UDP protocol.
 * It does two things: broadcast to all network interfaces trying
 * to discover open networks.
 * And reading of the broadcast port for others who have sent us messages.
 * If we receive a message, we open a new connection to that server.
 */
Client::Client(QString nickName)
{   
    myNickName = nickName;
    peerManager = new PeerManager(this);
    peerManager->setServerPort(server.serverPort());
    peerManager->startBroadcasting();

    QObject::connect(peerManager, SIGNAL(newConnection(Connection*)),
                     this, SLOT(newConnection(Connection*)));
    QObject::connect(&server, SIGNAL(newConnection(Connection*)),
                     this, SLOT(newConnection(Connection*)));
}

void Client::sendMessage(const QString &message)
{
    if (message.isEmpty())
        return;

    QList<Connection *> connections = peers.values();
    foreach (Connection *connection, connections)
        connection->sendMessage(message);
}
QString Client::getMyShortNick() {
    return myNickName;
}
QString Client::nickName() const
{
    return QString(peerManager->userName()) + '@' + QHostInfo::localHostName()
           + ':' + QString::number(server.serverPort());
}

bool Client::hasConnection(const QHostAddress &senderIp, int senderPort) const
{
    qDebug() << "hasConnection cd1";
    if (senderPort == -1)
        return peers.contains(senderIp);
    qDebug() << "hasConnection cd2";
    if (!peers.contains(senderIp))
        return false;
    qDebug() << "Sender Ip in peer";
    QList<Connection *> connections = peers.values(senderIp);
    foreach (Connection *connection, connections) {
        if (connection->peerPort() == senderPort)
            return true;
    }

    return false;
}
/* The server received a new connection:
 * We set the connection up by sending a greeting message
 * through the connection object which signals readyForUse()
 * upon which we add the connection to our peers */
void Client::newConnection(Connection *connection)
{
    connection->setGreetingMessage(peerManager->userName());

    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
}

void Client::readyForUse()
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->peerAddress(),
                                     connection->peerPort()))
        return;

    connect(connection, SIGNAL(newMessage(QString,QString)),
            this, SIGNAL(newMessage(QString,QString)));

    peers.insert(connection->peerAddress(), connection);
    QString nick = connection->name();
    if (!nick.isEmpty())
        emit newParticipant(nick);
}

void Client::disconnected()
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::connectionError(QAbstractSocket::SocketError /* socketError */)
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::removeConnection(Connection *connection)
{
    if (peers.contains(connection->peerAddress())) {
        peers.remove(connection->peerAddress());
        QString nick = connection->name();
        if (!nick.isEmpty())
            emit participantLeft(nick);
    }
    connection->deleteLater();
}
