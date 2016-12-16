#-------------------------------------------------
#
# Project created by QtCreator 2016-12-16T15:09:45
#
#-------------------------------------------------

QT       -= gui

TARGET = VideoCapture
TEMPLATE = lib

DEFINES += VIDEOCAPTURE_LIBRARY

SOURCES += videocapture.cpp

HEADERS += videocapture.h\
        videocapture_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
