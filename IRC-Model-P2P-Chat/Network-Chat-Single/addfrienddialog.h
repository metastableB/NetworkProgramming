#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AddFriendDialog;
}

class AddFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendDialog(QWidget *parent = 0);
    ~AddFriendDialog();

private:
    Ui::AddFriendDialog *ui;
signals:
    void okClicked(QString);
private slots:
    void slotOkClicked();
};

#endif // ADDFRIENDDIALOG_H
