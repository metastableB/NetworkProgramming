#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
private:
    bool isConnected;
public:
    bool getIsConnected();
    Client();
    QTcpSocket* socket;
    void connectTo(QString, QString);
    void close();
private slots:
    void disconnected();
    void connectionError(QAbstractSocket::SocketError /* socketError */);
    void readReady();
    void sendMessage(QString msg);
    void newConnection(QTcpSocket *t);
    void connected();
signals:
    void clientMessage(QString);
    void newConnectionClient(QTcpSocket *t);
};

#endif // CLIENT_H
