TEMPLATE = lib
TARGET = object_manager

QT += sql widgets gui

DESTDIR = ../build


INCLUDEPATH += . \
                ../include \
                ../my_mapaccess \
                ../reports \
                ../output

DEPENDPATH +=  . \
                ../my_mapaccess \
                ../output \
                ../reports
LIBS += -L../build -lreports -loutput

DEFINES += __USE_DLL_XXXXX
DEFINES += BUILD_DLL

include(object_manager.pri)
