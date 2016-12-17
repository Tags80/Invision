#-------------------------------------------------
#
# Project created by QtCreator 2016-12-16T15:09:45
#
#-------------------------------------------------

QT -= gui

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



win32 {

    ## Windows common build here

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")

        ## Windows x86 (32bit) specific build here

        LIBS += -L$$PWD/Libs/ffmpeg-win32-dev/lib/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswscale #-lswresample

        INCLUDEPATH += $$PWD/Libs/ffmpeg-win32-dev/include
        DEPENDPATH += $$PWD/Libs/ffmpeg-win32-dev/include

    } else {
        message("x86_64 build")

        ##Windows x64 (64 bit) specific build here
        LIBS += -L$$PWD/Libs/ffmpeg-win64-dev/lib/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswscale #-lswresample

        INCLUDEPATH += $$PWD/Libs/ffmpeg-win64-dev/include
        DEPENDPATH += $$PWD/Libs/ffmpeg-win64-dev/include

    }
}


