#-------------------------------------------------
#
# Project created by QtCreator 2019-01-18T14:24:24
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = qtcommission
TEMPLATE = app


SOURCES += main.cpp\
    ui/mainwindow.cpp \
    ui/videodisplayer.cpp \
    backend.cpp

HEADERS  += backend.h \
    ui/mainwindow.h \
    ui/videodisplayer.h 

FORMS    += ui/mainwindow.ui \
    ui/videodisplayer.ui

INCLUDEPATH += ui
include($$PWD/netcomm/netcomm.pri)
