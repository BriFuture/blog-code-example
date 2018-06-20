QT += core
QT -= gui

CONFIG += c++11

TARGET = xmlreader
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    instructparser.cpp \
    instruct.cpp

HEADERS += \
    instructparser.h \
    instruct.h
