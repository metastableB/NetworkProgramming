#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include "authentication.h"
#include <set>
#include <QMultiHash>

class Client : public QObject
{
    Q_OBJECT
private:
    bool isConnected;
    bool allowGroupChat;
    QString s_ip;
    quint16 s_port;
    Authentication* auth;
    QMultiHash<quint16,QTcpSocket*> peers;
public:
    bool getIsConnected();
    bool getGroupChatAllowed();
    Client();
    QTcpSocket* socket;
    void joinGroupAdmin(QString, quint16);
    void hostPeers();
    void connectTo(QString, QString);
    void close();
    void initGroupChat(Authentication*, QString s_ip, quint16 s_port);
    void joinGroupChat(QString,QString);
private slots:
    void disconnected();
    void connectionError(QAbstractSocket::SocketError /* socketError */);
    void readReady();
    void sendMessage(QString msg);
    void newConnection(QTcpSocket *t);
    void connected();
public slots:
    void createGroup(QString);
    void joinGroup(QString);
signals:
    void clientMessage(QString);
    void newConnectionClient(QTcpSocket *t);
};

#endif // CLIENT_H
