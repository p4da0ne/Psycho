TEMPLATE = lib
TARGET = object_manager

QT += sql widgets gui

DESTDIR = ../build

RESOURCES       += object_manager.qrc

INCLUDEPATH += . \
                ../include \
                ../my_mapaccess \
                ../reports \
                ../output

DEPENDPATH +=  . \
                ../my_mapaccess \
                ../output \
                ../reports
LIBS += -L../build -lreports -loutput -lmy_mapaccess

DEFINES += __USE_DLL_XXXXX
DEFINES += BUILD_DLL

include(object_manager.pri)
