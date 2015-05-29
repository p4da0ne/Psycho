TEMPLATE = lib

TARGET = event_manager

DESTDIR = ../build

INCLUDEPATH += . \
                ../include \
				../eventsmodel


DEPENDPATH += ../eventsmodel

LIBS += -L../build -leventsmodel

DEFINES += __USE_DLL_EVENT_MANAGER

#Include file(s)
include(event_manager.pri)
