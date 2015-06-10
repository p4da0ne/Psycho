TEMPLATE = lib

TARGET = event_manager

DESTDIR = ../build \

QT += sql

INCLUDEPATH += . \
                ../include \
                ../eventsmodel \
                ../coord


DEPENDPATH += ../eventsmodel \
                ../coord

LIBS += -L../build -leventsmodel -lcoord

DEFINES += __USE_DLL_EVENT_MANAGER

#Include file(s)
include(event_manager.pri)
