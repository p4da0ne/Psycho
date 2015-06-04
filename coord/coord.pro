TEMPLATE	= lib

TARGET		= coord

DESTDIR = ../build

QT += sql

INCLUDEPATH += ./ \
                ../include

DEFINES += __USE_DLL_COORD

include(coord.pri)

