#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(Authentication* a,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    auth = a;
    a->connect();
    ui->setupUi(this);
    loggedIn = true;
    QObject::connect(this->ui->closePushButton, SIGNAL(clicked()),
                     this,SLOT(exitApp()));
    QObject::connect(this->ui->loginPushButton, SIGNAL(clicked()),
                     this,SLOT(loginButtonPressed()));
    QObject::connect(this->ui->signupPushButton, SIGNAL(clicked()),
                     this,SLOT(signupButtonPressed()));
    QObject::connect(auth,SIGNAL(authenticationMessage(QString)),
                     this,SLOT(responseHandler(QString)));
}

void LoginDialog::disconnectAll(){
    QObject::disconnect(this->ui->closePushButton, SIGNAL(clicked()),
                     this,SLOT(exitApp()));
    QObject::disconnect(this->ui->loginPushButton, SIGNAL(clicked()),
                     this,SLOT(loginButtonPressed()));
    QObject::disconnect(this->ui->signupPushButton, SIGNAL(clicked()),
                     this,SLOT(signupButtonPressed()));
    QObject::disconnect(auth,SIGNAL(authenticationMessage(QString)),
                     this,SLOT(responseHandler(QString)));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::loginButtonPressed(){

    QString pwd = this->ui->passwordLineEdit->text();
    QString usr = this->ui->usernameLineEdit->text();

    if(pwd.isEmpty() || usr.isEmpty())
        messageBoxEcho("Error","Fields cannot be empty");
    else {
        auth->setCredentials(usr,pwd);
        auth->p_authenticate();
    }
}


void LoginDialog::signupButtonPressed(){
    QString pwd = this->ui->passwordLineEdit->text();
    QString usr = this->ui->usernameLineEdit->text();

    if(pwd.isEmpty() || usr.isEmpty())
        messageBoxEcho("Error","Fields cannot be empty");
    else{
        auth->setCredentials(usr,pwd);
        auth->p_register();
    }
}

void LoginDialog::responseHandler(QString msg){
    qDebug() << "Received msg here" << msg;
    qDebug() << msg.compare("AUTHENTICATE_SUCCESS");

    if(msg.compare("DISCONNECTED") == 0)
            messageBoxEcho("Error",msg);

    else if(msg.compare("CONNECTIONERROR") == 0)
        messageBoxEcho("Error",msg);


    else if(msg.compare("AUTHENTICATE_SUCCESS") == 0){
        qDebug() << "Emmiting logedin";
        //bool oldState = auth->blockSignals(true);
        //auth->close();
        //auth->blockSignals(oldState);
        emit loggedin();
    }else if(msg.contains("AUTHENTICATE_FAILURE")){
        QStringList l = msg.split("|");
        messageBoxEcho("Error",l[1]);
    }


    else if(msg.compare("REGISTER_SUCCESS") == 0){
        emit loggedin();
    } else if(msg.contains("REGISTER_FAILURE")){
        QStringList l = msg.split('|');
        messageBoxEcho("Error",l[1]);
    }


    else
       messageBoxEcho("Error",msg);
}

void LoginDialog::exitApp(){
    exit(1);
}

void LoginDialog::messageBoxEcho(QString type,QString msg){
    QMessageBox messageBox;
    messageBox.critical(0,type,msg);
    messageBox.setFixedSize(500,200);
}
