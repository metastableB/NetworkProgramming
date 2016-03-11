#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "authentication.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(Authentication* auth,QWidget *parent = 0);
    ~LoginDialog();
    void authenticate(QString,QString);
    bool loggedIn;
private:
    Ui::LoginDialog *ui;
    Authentication* auth;
    void messageBoxEcho(QString, QString);

private slots:
    void loginButtonPressed();
    void signupButtonPressed();
    void responseHandler(QString);
    void exitApp();
signals:
    void login(QString,QString);
    void signup(QString,QString);
    void returnedPressed();
    void loggedin();
public slots:
    void disconnectAll();
};

#endif // LOGINDIALOG_H
