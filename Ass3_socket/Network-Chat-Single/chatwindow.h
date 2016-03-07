#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <map>
#include <QModelIndex>
#include "client.h"
#include "server.h"
#include "authentication.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    std::map<QString,std::pair<QString,QString> > friends;
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();
    Authentication authServ;
    Server* server;
    Client* client;

private:
    Ui::ChatWindow *ui;
    void createFriendsList(QString l);
    void authenticationHelper();
    void chatInit();
    QString myNickName;
    bool isLoggedIn;
private slots:
    bool getLineEditMessage();
    bool friendSelected(QModelIndex);
    void authenticationMessage(QString msg);
    void handleMessage(QString s);

signals:
   void sendMessage(QString);
};

#endif // CHATWINDOW_H
