#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <map>
#include <QModelIndex>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <string>
#include <iostream>
#include <queue>

#define STDOUT 1
#define STDIN 0

#define BACKLOG 10	 // how many pending connections queue will hold
#define MAXDATASIZE 256

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
    char* bindport;
    void* server();
private:
    Ui::ChatWindow *ui;
    void createFriendsList(QString l);
    void client();
private slots:
    bool getLineEditMessage();
    bool friendSelected(QModelIndex);
};

#endif // CHATWINDOW_H
