#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "ui_chatdialog.h"
#include "client.h"
#include "authentication.h"
#include <QTcpSocket>

class ChatDialog : public QDialog, private Ui::ChatDialog
{
    Q_OBJECT

public:
    ChatDialog(QWidget *parent = 0);

public slots:
    void appendMessage(const QString &from, const QString &message);

private slots:
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void showInformation();
    void authenticationMessage(QString msg);
private:
    Client* client;
    QString myNickName;
    QTextTableFormat tableFormat;

    // Authentication
    bool isLoggedIn;
    Authentication authServ;
    void loginHelper();
    void clientInit();

};

#endif
