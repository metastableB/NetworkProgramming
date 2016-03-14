/********************************************************************************
** Form generated from reading UI file 'addfrienddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDFRIENDDIALOG_H
#define UI_ADDFRIENDDIALOG_H

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

class Ui_AddFriendDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *addPushButton;
    QPushButton *cancelPushButton;

    void setupUi(QDialog *AddFriendDialog)
    {
        if (AddFriendDialog->objectName().isEmpty())
            AddFriendDialog->setObjectName(QStringLiteral("AddFriendDialog"));
        AddFriendDialog->resize(317, 119);
        AddFriendDialog->setMinimumSize(QSize(317, 0));
        AddFriendDialog->setMaximumSize(QSize(317, 16777215));
        layoutWidget = new QWidget(AddFriendDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 295, 92));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        usernameLabel = new QLabel(layoutWidget);
        usernameLabel->setObjectName(QStringLiteral("usernameLabel"));
        usernameLabel->setMinimumSize(QSize(70, 0));
        usernameLabel->setMaximumSize(QSize(70, 16777215));

        horizontalLayout->addWidget(usernameLabel);

        usernameLineEdit = new QLineEdit(layoutWidget);
        usernameLineEdit->setObjectName(QStringLiteral("usernameLineEdit"));
        usernameLineEdit->setMinimumSize(QSize(213, 0));
        usernameLineEdit->setMaximumSize(QSize(213, 16777215));

        horizontalLayout->addWidget(usernameLineEdit);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        addPushButton = new QPushButton(layoutWidget);
        addPushButton->setObjectName(QStringLiteral("addPushButton"));

        horizontalLayout_2->addWidget(addPushButton);

        cancelPushButton = new QPushButton(layoutWidget);
        cancelPushButton->setObjectName(QStringLiteral("cancelPushButton"));

        horizontalLayout_2->addWidget(cancelPushButton);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(horizontalLayout_3);


        retranslateUi(AddFriendDialog);
        QObject::connect(cancelPushButton, SIGNAL(clicked()), AddFriendDialog, SLOT(close()));

        addPushButton->setDefault(true);


        QMetaObject::connectSlotsByName(AddFriendDialog);
    } // setupUi

    void retranslateUi(QDialog *AddFriendDialog)
    {
        AddFriendDialog->setWindowTitle(QApplication::translate("AddFriendDialog", "Dialog", 0));
        label->setText(QApplication::translate("AddFriendDialog", "Add Friend", 0));
        usernameLabel->setText(QApplication::translate("AddFriendDialog", "Username :", 0));
        addPushButton->setText(QApplication::translate("AddFriendDialog", "Add", 0));
        cancelPushButton->setText(QApplication::translate("AddFriendDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class AddFriendDialog: public Ui_AddFriendDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDFRIENDDIALOG_H
