HEADERS       = chatdialog.h \
                client.h \
                connection.h \
                peermanager.h \
                server.h \
    authentication.h
SOURCES       = chatdialog.cpp \
                client.cpp \
                connection.cpp \
                main.cpp \
                peermanager.cpp \
                server.cpp \
    authentication.cpp
FORMS         = chatdialog.ui
QT           += network widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/network-chat
INSTALLS += target


