/********************************************************************************
** Form generated from reading UI file 'chatwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWINDOW_H
#define UI_CHATWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWindow
{
public:
    QAction *actionAdd_Friend;
    QAction *actionExit;
    QAction *actionRefresh_Friend_List;
    QAction *actionExit_;
    QAction *actionLeave_Chat;
    QAction *actionJoin_Group_Chat;
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *messageEcho;
    QLineEdit *messageEdit;
    QVBoxLayout *verticalLayout_3;
    QListWidget *friendsList;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *SendButton;
    QSpacerItem *horizontalSpacer_2;
    QStatusBar *statusBar;
    QMenuBar *optionsMenuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *ChatWindow)
    {
        if (ChatWindow->objectName().isEmpty())
            ChatWindow->setObjectName(QStringLiteral("ChatWindow"));
        ChatWindow->resize(442, 371);
        actionAdd_Friend = new QAction(ChatWindow);
        actionAdd_Friend->setObjectName(QStringLiteral("actionAdd_Friend"));
        actionExit = new QAction(ChatWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionRefresh_Friend_List = new QAction(ChatWindow);
        actionRefresh_Friend_List->setObjectName(QStringLiteral("actionRefresh_Friend_List"));
        actionExit_ = new QAction(ChatWindow);
        actionExit_->setObjectName(QStringLiteral("actionExit_"));
        actionLeave_Chat = new QAction(ChatWindow);
        actionLeave_Chat->setObjectName(QStringLiteral("actionLeave_Chat"));
        actionJoin_Group_Chat = new QAction(ChatWindow);
        actionJoin_Group_Chat->setObjectName(QStringLiteral("actionJoin_Group_Chat"));
        centralWidget = new QWidget(ChatWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 10, 399, 291));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        messageEcho = new QTextEdit(layoutWidget);
        messageEcho->setObjectName(QStringLiteral("messageEcho"));
        messageEcho->setMaximumSize(QSize(16777215, 260));
        messageEcho->setReadOnly(true);

        verticalLayout_2->addWidget(messageEcho);

        messageEdit = new QLineEdit(layoutWidget);
        messageEdit->setObjectName(QStringLiteral("messageEdit"));

        verticalLayout_2->addWidget(messageEdit);


        horizontalLayout_4->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        friendsList = new QListWidget(layoutWidget);
        friendsList->setObjectName(QStringLiteral("friendsList"));
        friendsList->setMinimumSize(QSize(131, 0));
        friendsList->setMaximumSize(QSize(131, 16777215));

        verticalLayout_3->addWidget(friendsList);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        SendButton = new QPushButton(layoutWidget);
        SendButton->setObjectName(QStringLiteral("SendButton"));
        SendButton->setAutoDefault(true);

        horizontalLayout->addWidget(SendButton);

        horizontalSpacer_2 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(verticalLayout);


        horizontalLayout_4->addLayout(verticalLayout_3);

        ChatWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ChatWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ChatWindow->setStatusBar(statusBar);
        optionsMenuBar = new QMenuBar(ChatWindow);
        optionsMenuBar->setObjectName(QStringLiteral("optionsMenuBar"));
        optionsMenuBar->setGeometry(QRect(0, 0, 442, 20));
        optionsMenuBar->setContextMenuPolicy(Qt::ActionsContextMenu);
        optionsMenuBar->setLayoutDirection(Qt::LeftToRight);
        optionsMenuBar->setDefaultUp(true);
        menuFile = new QMenu(optionsMenuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuFile->setFocusPolicy(Qt::NoFocus);
        ChatWindow->setMenuBar(optionsMenuBar);

        optionsMenuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionAdd_Friend);
        menuFile->addAction(actionLeave_Chat);
        menuFile->addAction(actionJoin_Group_Chat);
        menuFile->addAction(actionRefresh_Friend_List);
        menuFile->addSeparator();
        menuFile->addAction(actionExit_);

        retranslateUi(ChatWindow);
        QObject::connect(SendButton, SIGNAL(clicked()), ChatWindow, SLOT(getLineEditMessage()));
        QObject::connect(friendsList, SIGNAL(doubleClicked(QModelIndex)), ChatWindow, SLOT(friendSelected(QModelIndex)));
        QObject::connect(messageEdit, SIGNAL(returnPressed()), SendButton, SLOT(click()));

        SendButton->setDefault(true);


        QMetaObject::connectSlotsByName(ChatWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChatWindow)
    {
        ChatWindow->setWindowTitle(QApplication::translate("ChatWindow", "ChatWindow", 0));
        actionAdd_Friend->setText(QApplication::translate("ChatWindow", "Add Friend", 0));
        actionExit->setText(QApplication::translate("ChatWindow", "Exit", 0));
        actionRefresh_Friend_List->setText(QApplication::translate("ChatWindow", "Refresh Friend List", 0));
        actionExit_->setText(QApplication::translate("ChatWindow", "Exit", 0));
        actionLeave_Chat->setText(QApplication::translate("ChatWindow", "Leave Chat", 0));
        actionJoin_Group_Chat->setText(QApplication::translate("ChatWindow", "Join Group Chat", 0));
        messageEdit->setPlaceholderText(QApplication::translate("ChatWindow", "Enter message", 0));
        SendButton->setText(QApplication::translate("ChatWindow", "Send", 0));
        menuFile->setTitle(QApplication::translate("ChatWindow", "Options", 0));
    } // retranslateUi

};

namespace Ui {
    class ChatWindow: public Ui_ChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWINDOW_H
