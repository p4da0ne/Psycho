TEMPLATE = lib

TARGET = dataaccess

DESTDIR = ../build

QT += sql

INCLUDEPATH +=

DEFINES += __USE_DLL_XXXXX
DEFINES += BUILD_DLL

#Include file(s)
include(dataaccess.pri)
