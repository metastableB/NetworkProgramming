#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H


#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>
#include <QAbstractSocket>

#define AUTH_SERV "127.0.0.1"
#define AUTH_PORT 3501

class Authentication : public QTcpSocket
{
    Q_OBJECT

public:

    Authentication(QObject *parent = 0);
    void setCredentials(QString uname, QString pwd);
    void registerUser(QString,QString);
    void connect();
    QString getUsername();
    /* Protocol Function */
    void p_authenticate();
    void p_register();
    void p_addFriend(QString);
    void p_postMyIp(QString ip, quint16);
    void p_getFriendList();
signals:
    void authenticationMessage(QString msg);
private:
    QString username;
    QString password;
    bool isCredentialsSet;
private slots:
    void readyRead();
    void protoWrite(QString);
    void disconnected();
    void connected();
    void connectionError(QAbstractSocket::SocketError);
};

#endif // AUTHENTICATION_H

