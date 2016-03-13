#ifndef JOINGROUPDIALOG_H
#define JOINGROUPDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class joinGroupDialog;
}

class joinGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit joinGroupDialog(QWidget *parent = 0);
    ~joinGroupDialog();

private:
    Ui::joinGroupDialog *ui;
private slots:
    void slotJoinPressed();
    void slotCreatePressed();
signals:
    void joinGroup(QString);
    void createGroup(QString);
};

#endif // JOINGROUPDIALOG_H
