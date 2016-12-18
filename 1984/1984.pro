#-------------------------------------------------
#
# Project created by QtCreator 2016-12-16T15:14:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 1984
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    mcroplabel.cpp

HEADERS  += mainwindow.h \
    mcroplabel.h

FORMS    += mainwindow.ui

#include(../qxtglobalshortcut5/qxt.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../VideoCapture/release/ -lVideoCapture
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../VideoCapture/debug/ -lVideoCapture
else:unix: LIBS += -L$$OUT_PWD/../VideoCapture/ -lVideoCapture

INCLUDEPATH += $$PWD/../VideoCapture
DEPENDPATH += $$PWD/../VideoCapture


DISTFILES += res/img/icon.png

RESOURCES += \
    images.qrc
