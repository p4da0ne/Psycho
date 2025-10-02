TEMPLATE = lib

TARGET = reports

DESTDIR = ../build

QT += sql widgets printsupport core5compat

INCLUDEPATH += . \
                ../output \
                ../textedit \
                ../Calculate_K_omkrf

DEPENDPATH +=   ../output \
                ../textedit \
                ../Calculate_K_omkrf

LIBS += -L../build -loutput -ltextedit

DEFINES += __USE_DLL_REPORTS

#Include file(s)
include(reports.pri)
