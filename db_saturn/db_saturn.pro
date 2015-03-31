TEMPLATE = lib

TARGET = db_saturn

DESTDIR = ../build

QT += sql

INCLUDEPATH += ../dataaccess

DEFINES += __USE_DLL_XXXXX

LIBS += -L../build -ldataaccess

#Include file(s)
include(db_saturn.pri)

#DEPENDPATH += ../dataaccess




