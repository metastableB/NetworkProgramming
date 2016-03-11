#include "chatwindow.h"
#include <QApplication>
#include "logindialog.h"
#include "authentication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Authentication* auth = new Authentication();
    LoginDialog l(auth);
    auth->setCredentials("don","met");
    l.show();
    ChatWindow w(auth);
    //auth->p_authenticate();
    //w.show();
    QObject::connect(&l, SIGNAL(loggedin()),
                     &l, SLOT(close()));
    QObject::connect(&l, SIGNAL(loggedin()),
                     &w, SLOT(chatInit()));
    QObject::connect(&l, SIGNAL(loggedin()),
                     &w, SLOT(show()));
    QObject::connect(&l,SIGNAL(loggedin()),
                     &l,SLOT(disconnectAll()));
    return a.exec();
}
