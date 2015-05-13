TEMPLATE = lib

TARGET = reports

DESTDIR = ../build

QT += sql

INCLUDEPATH += . \
                ../output \
                ../textedit

DEPENDPATH +=   ../output \
                ../textedit

LIBS += -L../build -loutput -ltextedit

DEFINES += __USE_DLL_REPORTS

#Include file(s)
include(reports.pri)
