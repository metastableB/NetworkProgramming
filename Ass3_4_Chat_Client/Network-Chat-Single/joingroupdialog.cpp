#include "joingroupdialog.h"
#include "ui_joingroupdialog.h"

joinGroupDialog::joinGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::joinGroupDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->createPushButton,SIGNAL(clicked()),
                     this,SLOT(slotCreatePressed()));
    QObject::connect(ui->joinPushButton,SIGNAL(clicked()),
                     this,SLOT(slotJoinPressed()));
}

joinGroupDialog::~joinGroupDialog()
{
    delete ui;
}

void joinGroupDialog::slotCreatePressed(){
    QString group = ui->groupNameLineEdit->text();
    if(group.isEmpty()){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Group name cannot be empty.");
        messageBox.setFixedSize(500,200);
    }else {
        close();
        emit createGroup(group);
    }
}


void joinGroupDialog::slotJoinPressed(){
    QString group = ui->groupNameLineEdit->text();
    if(group.isEmpty()){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Group name cannot be empty.");
        messageBox.setFixedSize(500,200);
    }else {
        close();
        emit joinGroup(group);
    }
}
