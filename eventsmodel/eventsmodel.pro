TEMPLATE = lib

TARGET = eventsmodel

DESTDIR = ../build

QT += sql


DEFINES += __USE_DLL_EVENTSMODEL
DEFINES += BUILD_DLL


INCLUDEPATH += ./ \
               ../coord



DEPENDPATH += 	./ \
                ../coord

LIBPATH   += ../build

LIBS += -lcoord


#Include file(s)
include(eventsmodel.pri)
