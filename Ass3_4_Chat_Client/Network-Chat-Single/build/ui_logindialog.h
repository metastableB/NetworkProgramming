/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *usenameLabel;
    QLabel *passwordLabel;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *closePushButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *signupPushButton;
    QPushButton *loginPushButton;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName(QStringLiteral("LoginDialog"));
        LoginDialog->resize(414, 137);
        widget = new QWidget(LoginDialog);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 391, 113));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        usenameLabel = new QLabel(widget);
        usenameLabel->setObjectName(QStringLiteral("usenameLabel"));

        verticalLayout->addWidget(usenameLabel);

        passwordLabel = new QLabel(widget);
        passwordLabel->setObjectName(QStringLiteral("passwordLabel"));

        verticalLayout->addWidget(passwordLabel);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        usernameLineEdit = new QLineEdit(widget);
        usernameLineEdit->setObjectName(QStringLiteral("usernameLineEdit"));
        usernameLineEdit->setFocusPolicy(Qt::StrongFocus);

        verticalLayout_2->addWidget(usernameLineEdit);

        passwordLineEdit = new QLineEdit(widget);
        passwordLineEdit->setObjectName(QStringLiteral("passwordLineEdit"));

        verticalLayout_2->addWidget(passwordLineEdit);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        closePushButton = new QPushButton(widget);
        closePushButton->setObjectName(QStringLiteral("closePushButton"));

        horizontalLayout_2->addWidget(closePushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        signupPushButton = new QPushButton(widget);
        signupPushButton->setObjectName(QStringLiteral("signupPushButton"));

        horizontalLayout_2->addWidget(signupPushButton);

        loginPushButton = new QPushButton(widget);
        loginPushButton->setObjectName(QStringLiteral("loginPushButton"));

        horizontalLayout_2->addWidget(loginPushButton);


        verticalLayout_3->addLayout(horizontalLayout_2);

        usernameLineEdit->raise();
        usenameLabel->raise();
        passwordLabel->raise();
        passwordLineEdit->raise();
        loginPushButton->raise();
        signupPushButton->raise();
        signupPushButton->raise();
        QWidget::setTabOrder(usernameLineEdit, passwordLineEdit);
        QWidget::setTabOrder(passwordLineEdit, loginPushButton);
        QWidget::setTabOrder(loginPushButton, signupPushButton);
        QWidget::setTabOrder(signupPushButton, closePushButton);

        retranslateUi(LoginDialog);
        QObject::connect(usernameLineEdit, SIGNAL(returnPressed()), passwordLineEdit, SLOT(setFocus()));

        loginPushButton->setDefault(true);


        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QApplication::translate("LoginDialog", "Dialog", 0));
        usenameLabel->setText(QApplication::translate("LoginDialog", "Usename :", 0));
        passwordLabel->setText(QApplication::translate("LoginDialog", "Password :", 0));
        closePushButton->setText(QApplication::translate("LoginDialog", "Close", 0));
        signupPushButton->setText(QApplication::translate("LoginDialog", "Signup", 0));
        loginPushButton->setText(QApplication::translate("LoginDialog", "Login", 0));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
