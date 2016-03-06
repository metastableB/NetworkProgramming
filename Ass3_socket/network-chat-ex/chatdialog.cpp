/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "chatdialog.h"

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    lineEdit->setFocusPolicy(Qt::StrongFocus);
    textEdit->setFocusPolicy(Qt::NoFocus);
    textEdit->setReadOnly(true);
    listWidget->setFocusPolicy(Qt::NoFocus);

    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    isLoggedIn = false;
    if(!isLoggedIn)
        loginHelper();
}

void ChatDialog::clientInit(){
    client = new Client(myNickName);
    newParticipant(myNickName);
    tableFormat.setBorder(0);
    //QTimer::singleShot(10 * 1000, this, SLOT(showInformation()));
    connect(client, SIGNAL(newMessage(QString,QString)),
            this, SLOT(appendMessage(QString,QString)));
    connect(client, SIGNAL(newParticipant(QString)),
            this, SLOT(newParticipant(QString)));
    connect(client, SIGNAL(participantLeft(QString)),
            this, SLOT(participantLeft(QString)));
}

void ChatDialog::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;
    QTextCursor cursor(textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    if(from == myNickName)
        textEdit->append("<font color=green>"+from +": "+ message+"</font>");
    else
        textEdit->append("<font color=IndianRed>"+ from +": "+ message+"</font>");
    QScrollBar *bar = textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatDialog::returnPressed()
{
    QString text = lineEdit->text();
    if (text.isEmpty())
        return;
    if (text.startsWith(QChar('/'))) {
        if(text.compare("/xkcd") == 0){
            QColor color = textEdit->textColor();
            textEdit->setTextColor(Qt::blue);
            textEdit->append(tr("!Aren't you a smart one!"));
            textEdit->setTextColor(color);    
        } else {
            QStringList l = text.split(QChar(' '));
            if(l[0].compare("/login") == 0 && !isLoggedIn){
                if(l.size() != 2) {
                    QColor color = textEdit->textColor();
                    textEdit->setTextColor(Qt::red);
                    textEdit->append(tr("! Invalid Usage: /login"));
                    textEdit->setTextColor(color);
                    loginHelper();
                    lineEdit->clear();
                    return;
                }
                myNickName = l.at(1);
                QColor color = textEdit->textColor();
                textEdit->setTextColor(Qt::blue);
                textEdit->append(tr("Authenticating: %1")
                    .arg(myNickName));
                textEdit->setTextColor(color);    
                isLoggedIn = true;
                clientInit();
            } else {
                QColor color = textEdit->textColor();
                textEdit->setTextColor(Qt::red);
                textEdit->append(tr("! Unknown command or Invalid usage: %1")
                                 .arg(text.left(text.indexOf(' '))));
                textEdit->setTextColor(color);
            }
        }
    } else if(isLoggedIn){
        client->sendMessage(text);
        appendMessage(myNickName, text);
    } else loginHelper();

    lineEdit->clear();
}

void ChatDialog::newParticipant(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QColor color = textEdit->textColor();
    textEdit->setTextColor(Qt::gray);
    textEdit->append(tr("* %1 has joined").arg(nick));
    textEdit->setTextColor(color);
    listWidget->addItem(nick);
}

void ChatDialog::participantLeft(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QList<QListWidgetItem *> items = listWidget->findItems(nick,
                                                           Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
    QColor color = textEdit->textColor();
    textEdit->setTextColor(Qt::gray);
    textEdit->append(tr("* %1 has left").arg(nick));
    textEdit->setTextColor(color);
}

void ChatDialog::showInformation()
{
    if (listWidget->count() == 1) {
        QMessageBox::information(this, tr("Chat"),
                                 tr("Launch several instances of this "
                                    "program on your local network and "
                                    "start chatting!"));
    }
}

void ChatDialog::loginHelper()
{
    QColor color = textEdit->textColor();
    textEdit->setTextColor(Qt::blue);
    textEdit->append(tr("You need to authenticate first"));
    textEdit->append(tr("Use /login <username> <password>"));
    textEdit->setTextColor(color); 
}
