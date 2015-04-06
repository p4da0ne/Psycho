TEMPLATE = lib

TARGET = reports

DESTDIR = ../build

QT += sql

INCLUDEPATH += . \
                ../output

DEPENDPATH +=  ../output

LIBS += -L../build -loutput

DEFINES += __USE_DLL_REPORTS

#Include file(s)
include(reports.pri)
