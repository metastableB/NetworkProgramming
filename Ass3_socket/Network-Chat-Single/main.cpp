#include "chatwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatWindow w;
    w.show();
    a.exec();
    return 0;
}
