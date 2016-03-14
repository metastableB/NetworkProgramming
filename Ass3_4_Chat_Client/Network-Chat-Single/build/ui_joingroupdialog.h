/********************************************************************************
** Form generated from reading UI file 'joingroupdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JOINGROUPDIALOG_H
#define UI_JOINGROUPDIALOG_H

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

class Ui_joinGroupDialog
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLineEdit *groupNameLineEdit;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *joinPushButton;
    QPushButton *createPushButton;
    QPushButton *cancelPushButton;

    void setupUi(QDialog *joinGroupDialog)
    {
        if (joinGroupDialog->objectName().isEmpty())
            joinGroupDialog->setObjectName(QStringLiteral("joinGroupDialog"));
        joinGroupDialog->resize(290, 109);
        widget = new QWidget(joinGroupDialog);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 261, 85));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        groupNameLineEdit = new QLineEdit(widget);
        groupNameLineEdit->setObjectName(QStringLiteral("groupNameLineEdit"));
        groupNameLineEdit->setClearButtonEnabled(false);

        verticalLayout_2->addWidget(groupNameLineEdit);


        verticalLayout_3->addLayout(verticalLayout_2);

        verticalSpacer = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        joinPushButton = new QPushButton(widget);
        joinPushButton->setObjectName(QStringLiteral("joinPushButton"));

        verticalLayout->addWidget(joinPushButton);

        createPushButton = new QPushButton(widget);
        createPushButton->setObjectName(QStringLiteral("createPushButton"));

        verticalLayout->addWidget(createPushButton);

        cancelPushButton = new QPushButton(widget);
        cancelPushButton->setObjectName(QStringLiteral("cancelPushButton"));

        verticalLayout->addWidget(cancelPushButton);


        horizontalLayout->addLayout(verticalLayout);

        groupNameLineEdit->raise();
        label_2->raise();
        groupNameLineEdit->raise();
        cancelPushButton->raise();
        cancelPushButton->raise();
        groupNameLineEdit->raise();
        label_2->raise();

        retranslateUi(joinGroupDialog);

        joinPushButton->setDefault(true);


        QMetaObject::connectSlotsByName(joinGroupDialog);
    } // setupUi

    void retranslateUi(QDialog *joinGroupDialog)
    {
        joinGroupDialog->setWindowTitle(QApplication::translate("joinGroupDialog", "Dialog", 0));
        label_2->setText(QApplication::translate("joinGroupDialog", "Group Name", 0));
        joinPushButton->setText(QApplication::translate("joinGroupDialog", "Join Group", 0));
        createPushButton->setText(QApplication::translate("joinGroupDialog", "Create Group", 0));
        cancelPushButton->setText(QApplication::translate("joinGroupDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class joinGroupDialog: public Ui_joinGroupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JOINGROUPDIALOG_H
