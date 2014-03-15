#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T21:47:42
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = projekt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connectionwidget.cpp \
    container.cpp

HEADERS  += mainwindow.h \
    connectionwidget.h \
    container.h

FORMS    += mainwindow.ui \
    connectionwidget.ui

RESOURCES += \
    images.qrc
