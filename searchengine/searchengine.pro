TEMPLATE = lib

TARGET = searchengine

DESTDIR = ../build

QT += sql

INCLUDEPATH += ./ \
				../include

DEFINES += __USE_DLL_SEARCHENGINE

#Include file(s)
include(searchengine.pri)
