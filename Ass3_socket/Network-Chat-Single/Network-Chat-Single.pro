#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T20:27:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Network-Chat-Single
TEMPLATE = app


SOURCES += main.cpp\
        chatwindow.cpp\
        server.cpp \
    client.cpp \
    authentication.cpp \
    logindialog.cpp \
    addfrienddialog.cpp \
    joingroupdialog.cpp

HEADERS  += chatwindow.h\
            server.h\
    client.h \
    authentication.h \
    logindialog.h \
    addfrienddialog.h \
    joingroupdialog.h

FORMS    += chatwindow.ui \
    logindialog.ui \
    addfrienddialog.ui \
    joingroupdialog.ui
