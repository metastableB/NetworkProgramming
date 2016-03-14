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
    connect(&authServ, SIGNAL(authenticationMessage(QString)),
            this, SLOT(authenticationMessage(QString)));
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

void ChatDialog::authenticationMessage(QString msg){
    // if teh message is good: we proceede clientInit();
    QColor color = textEdit->textColor();
    qDebug () << msg;
    if(msg.compare("DISCONNECTED") == 0){
        textEdit->setTextColor(Qt::red);
        textEdit->append("Disconnected from AuthServ");
    } else if(msg.compare("CONNECTIONERROR") == 0){
        textEdit->setTextColor(Qt::red);
        textEdit->append("ERROR: Connection Error");
    } else if(msg.compare("AUTHENTICATE_SUCCESS") == 0){
        isLoggedIn = true;
        textEdit->setTextColor(Qt::green);
        textEdit->append("Successfully authenticated.\nDisconnecting from AuthServ");
        authServ.close();
        clientInit();
    } else {
        textEdit->setTextColor(Qt::green);
        textEdit->append(msg);
    }
    textEdit->setTextColor(color);
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
                if(l.size() != 3) {
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
                
                if(!isLoggedIn){
                   authServ.setCredentials(l.at(1),l.at(2));
                   authServ.authenticate();
                }
                textEdit->setTextColor(color);    
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
