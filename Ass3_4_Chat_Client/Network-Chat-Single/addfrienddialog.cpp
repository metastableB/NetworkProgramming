#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"

AddFriendDialog::AddFriendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFriendDialog)
{
    ui->setupUi(this);
    QObject::connect(this->ui->addPushButton,SIGNAL(clicked(bool)),
                                                    this,SLOT(slotOkClicked()));
}

AddFriendDialog::~AddFriendDialog()
{
    delete ui;
}

void AddFriendDialog::slotOkClicked(){
    QString u = ui->usernameLineEdit->text();
    if(u.isEmpty()){
        QMessageBox *a = new QMessageBox();
        a->critical(0,"Error","Username cannot be empty");
        a->setFixedSize(500,200);
    }else{
        close();
        emit okClicked(u);
    }
}
