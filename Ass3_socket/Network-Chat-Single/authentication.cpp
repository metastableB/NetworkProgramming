#include "authentication.h"

/* The connection object is only initialized
 * IT does not have any controll structure moves.
 */
Authentication::Authentication(QObject *parent)
    : QTcpSocket(parent)
{
    username = "unknown";
    password = "unknown";
    isCredentialsSet = false;
    QObject::connect(this, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(this, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
}
void Authentication::authenticate(){
    qDebug() << "Authenticating";
    emit authenticationMessage("Connecting to authentication server");
    connectToHost(AUTH_SERV,AUTH_PORT);
    qDebug() << "You say what ?";
}

void Authentication::connected(){
    if(!isCredentialsSet){
        qDebug() << "FATAL: isCredentials set is false. This should not happend";
        exit(1);
    }
    qDebug() << "Connected";
    emit authenticationMessage("Connected\nAttempting to Login");
    QObject::connect(this,SIGNAL(readyRead()), this, SLOT(readyRead()));
    readyWrite();
}

void Authentication::readyRead(){
    qDebug() << "Ready read emitted";
    QByteArray data;
    data.resize(256);
    data = readAll();
    qDebug() << "Received " << data;
    emit authenticationMessage((data));
}

void Authentication::readyWrite(){
    QString data("AUTHENTICATE|"+username);
    data.append('|');
    data.append(password);
    write(data.toLatin1().data(),data.length());
}

void Authentication::setCredentials(QString uname, QString pwd){
    username = uname;
    password = pwd;
    isCredentialsSet = true;
}

void Authentication::disconnected() {
    emit authenticationMessage("DISCONNECTED");
}

void Authentication::connectionError(QAbstractSocket::SocketError /* socketError */) {
    emit authenticationMessage("CONNECTIONERROR");
}
