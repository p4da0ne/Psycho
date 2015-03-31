TEMPLATE = lib

TARGET = output

DESTDIR = ../build

QT += sql

INCLUDEPATH += . \

DEFINES += __USE_DLL_XXXXX

#Include file(s)
include(output.pri)
