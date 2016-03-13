#include "authentication.h"

/* TODO : fix the connected object vs allowed group chat object.
 * only the server needs to override it
 * clients need not
 */

/* The connection object is only initialized
 * IT does not have any controll structure moves.
 * Make sure all communicatin to the authentication server goes through here.
 *
 * Mark protocol rules here
 *
 * AUTHENTICATE|username|password
 * >> AUTHENTICATE_SUCCESSS or AUTHENTICATE_FAILURE
 *
 * REGISTER|username|password
 * >> REGISTER_SUCCESS or REGISTER_FAILURE
 *
 * ADD_FRIEND|friendname
 * >> ADD_FREIND_SUCCESS | ADDFRIEND_FAILURE
 *
 * GET_FRIEND_LIST|username
 * >> FRIENDLIST|[username|ip|port]*
 *
 * POST_MY_IP|username|ip|port
 * >> POST_MY_IP_SUCCESS | POST_MY_IP_FAILURE
 *
 * JOIN_GROUP|my_username|group_name
 * >> JOIN_GROUP|group_name|ip|port|[LIST OF GROUP MEMBERS]
 *
 * CREATE_GROUP|groupname|ip|port
 * >> CREATE_GROUP_SUCCESS|groupname or CREATE_GROUP_FAIURE
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

void Authentication::connect(){
   connectToHost(AUTH_SERV,AUTH_PORT);
}

void Authentication::connected(){
    if(!isCredentialsSet){
        qDebug() << "FATAL: isCredentials set is false. This should not happend";
        exit(1);
    }
    qDebug() << "Connected";
    qDebug() <<"Connected\nAttempting to Login";
    QObject::connect(this,SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Authentication::readyRead(){
    qDebug() << "Ready read emitted";
    QByteArray data;
    data.resize(256);
    data = readAll();
    qDebug() << "Received " << data;
    emit authenticationMessage((data));
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

void Authentication::protoWrite(QString data){
    write(data.toLatin1().data(),data.length());
}

QString Authentication::getUsername(){
    return username;
}

/*
 * Protocol functions
 */


void Authentication::p_authenticate(){
    qDebug() << "Authenticating" << AUTH_SERV << ":" << AUTH_PORT;
    protoWrite("AUTHENTICATE|"+username+"|"+password);
}

void Authentication::p_register(){
    qDebug() << "Registering user";
    protoWrite("REGISTER|"+username+"|"+password);
}

void Authentication::p_addFriend(QString uname){
    qDebug() << "Add Friend" << uname;
    protoWrite("ADD_FRIEND|"+uname);
}

void Authentication::p_postMyIp(QString ip, quint16 port){
    QString data("POST_MY_IP|"+username + '|' + ip +'|' + QString(QString::number(port)));
    protoWrite(data);
}

void Authentication::p_getFriendList(){
    QString data("GET_FRIEND_LIST|"+username);
    protoWrite(data);
}

void Authentication::p_joinGroup(QString grp){
    QString data("JOIN_GROUP|"+username+"|"+grp);
    protoWrite(data);
}

void Authentication::p_createGroup(QString grp, QString ip, quint16 port){
    QString data("CREATE_GROUP|"+grp+"|"+ip+"|"+QString(QString::number(port)));
    protoWrite(data);
}
