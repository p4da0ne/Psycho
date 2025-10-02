TEMPLATE = lib

TARGET = eventsmodel

DESTDIR = ../build

QT += sql widgets


DEFINES += __USE_DLL_EVENTSMODEL
DEFINES += BUILD_DLL


INCLUDEPATH += ./ \
                ../include \
               ../coord



DEPENDPATH += 	./ \
                ../coord

LIBS   += -L../build

LIBS += -lcoord


#Include file(s)
include(eventsmodel.pri)
