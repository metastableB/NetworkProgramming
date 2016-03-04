#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

bool ChatWindow::getLineEditMessage(){
    QString s = this->ui->messageEdit->text();
    if(s.length() == 0) return true;
    QString uname = "<font color=red>metastableB </font>";
    s = uname + s;
    this->ui->messageEcho->append(s);
    this->ui->messageEdit->clear();
    return true;
}
