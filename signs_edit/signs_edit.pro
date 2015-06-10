TEMPLATE = lib

TARGET = signs_edit

DESTDIR = ../build

QT += sql

INCLUDEPATH += . \
                ../include \
                ../my_mapaccess


DEPENDPATH += ../my_mapaccess

LIBS += -L../build -lmy_mapaccess

DEFINES += __USE_DLL_SIGNS_EDIT

#Include file(s)
include(signs_edit.pri)
