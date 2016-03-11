#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <map>
#include <QModelIndex>
#include <QAction>
#include <QDialog>
#include "client.h"
#include "server.h"
#include "authentication.h"
#include "addfrienddialog.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    std::map<QString,std::pair<QString,QString> > friends;
    explicit ChatWindow(Authentication* auth,QWidget *parent = 0);
    ~ChatWindow();
    Authentication* authServ;
    Server* server;
    Client* client;

private:
    Ui::ChatWindow *ui;
    void createFriendsList(QString l);
    QString myNickName;
    bool isLoggedIn;
    bool postMyIpSuccess;
    bool getFriendListSuccess;
private slots:
    void chatInit();
    bool getLineEditMessage();
    bool friendSelected(QModelIndex);
    void authenticationMessage(QString msg);
    void handleMessage(QString s);
    void slotOptionsMenu(QAction*);
    void addFriendHandler(QString);

signals:
   void sendMessage(QString);
};

#endif // CHATWINDOW_H
