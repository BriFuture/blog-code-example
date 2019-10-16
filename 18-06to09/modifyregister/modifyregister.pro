QT += core printsupport
QT -= gui

CONFIG += c++11

TARGET = modifyregister
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    registermodifier.cpp

HEADERS += \
    registermodifier.h

win32 {
#  QMAKE_POST_LINK += mt -manifest $$PWD/manifest.xml -outputresource:$$OUT_PWD/debug/$$TARGET".exe" $$escape_expand(\n\t)
  QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
}

DISTFILES += manifest.xml
VERSION = 1.0.0.0
DESTDIR = $$OUT_PWD/app
