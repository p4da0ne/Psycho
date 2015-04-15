TEMPLATE = lib
TARGET = object_manager

QT += sql widgets gui

DESTDIR = ../build


INCLUDEPATH += . \
                 ../reports \
                 ../output

DEPENDPATH +=  . \
                ../output \
                ../reports
LIBS += -L../build -lreports -loutput

DEFINES += __USE_DLL_XXXXX
DEFINES += BUILD_DLL

include(object_manager.pri)
