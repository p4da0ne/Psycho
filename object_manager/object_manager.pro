TEMPLATE = lib
TARGET = object_manager

QT += sql widgets gui

DESTDIR = ../build


INCLUDEPATH +=

DEFINES += __USE_DLL_XXXXX

include(object_manager.pri)
