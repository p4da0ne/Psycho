TEMPLATE = lib

TARGET = eventsmodel

DESTDIR = ../build

QT += sql


DEFINES += __USE_DLL_EVENTSMODEL
DEFINES += BUILD_DLL

#Include file(s)
include(eventsmodel.pri)
