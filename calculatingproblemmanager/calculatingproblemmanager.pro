TEMPLATE = lib
TARGET = calculatingproblemmanager

DESTDIR = ../build

QT     += sql gui widgets

INCLUDEPATH += . \

#LIBPATH   += ../build

DEFINES     += BUILD_DLL
DEFINES += __USE_DLL_XXXXX

include(calculatingproblemmanager.pri)
