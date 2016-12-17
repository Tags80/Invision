#-------------------------------------------------
#
# Project created by QtCreator 2016-12-16T15:09:45
#
#-------------------------------------------------


TARGET = VideoCapture
TEMPLATE = lib

DEFINES += VIDEOCAPTURE_LIBRARY

SOURCES += videocapture.cpp \
    grabbers/abstractframecapture.cpp \
    grabbers/standardscreencapture.cpp \
    stage.cpp \
    grabbers/cameracapture.cpp \
    encoders/frameencoder.cpp

HEADERS += videocapture.h\
        videocapture_global.h \
    grabbers/abstractframecapture.h \
    grabbers/standardscreencapture.h \
    stage.h \
    grabbers/cameracapture.h \
    encoders/frameencoder.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}



win32 {

    ## Windows common build here

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 Windows build")

        ## Windows x86 (32bit) specific build here

        LIBS += -L$$PWD/Libs/ffmpeg-win32-dev/lib/ -lavcodec -lavfilter -lavformat -lavutil -lswscale #-lswresample

        INCLUDEPATH += $$PWD/Libs/ffmpeg-win32-dev/include
        DEPENDPATH += $$PWD/Libs/ffmpeg-win32-dev/include

    } else {
        message("x86_64 Windows build")

        ##Windows x64 (64 bit) specific build here
        LIBS += -L$$PWD/Libs/ffmpeg-win64-dev/lib/ -lavcodec -lavfilter -lavformat -lavutil -lswscale #-lswresample

        INCLUDEPATH += $$PWD/Libs/ffmpeg-win64-dev/include
        DEPENDPATH += $$PWD/Libs/ffmpeg-win64-dev/include

    }
}



unix:!macx{

    LIBS += -lavcodec -lavfilter -lavformat -lavutil -lswscale
    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 Linux Build")

    } else {
        message("x86_64 Linux Build")
    }

}
