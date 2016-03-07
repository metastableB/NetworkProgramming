#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H


#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>
#include <QAbstractSocket>

#define AUTH_SERV "localhost"
#define AUTH_PORT 3500

class Authentication : public QTcpSocket
{
    Q_OBJECT

public:

    Authentication(QObject *parent = 0);
    void setCredentials(QString uname, QString pwd);
    void authenticate();
signals:
    void authenticationMessage(QString msg);
private:
    QString username;
    QString password;
    bool isCredentialsSet;
private slots:
    void readyRead();
    void readyWrite();
    void disconnected();
    void connected();
    void connectionError(QAbstractSocket::SocketError);
};

#endif // AUTHENTICATION_H

