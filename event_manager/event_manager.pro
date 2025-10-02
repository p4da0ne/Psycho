TEMPLATE = lib
TARGET = event_manager

DESTDIR = ../build

QT += sql widgets core5compat

INCLUDEPATH += . \
                ../include \
                ../eventsmodel \
                ../coord


DEPENDPATH += ../eventsmodel \
                ../coord

LIBS += -L../build -leventsmodel -lcoord

DEFINES += __USE_DLL_EVENT_MANAGER
DEFINES += BUILD_DLL

#Include file(s)
include(event_manager.pri)
