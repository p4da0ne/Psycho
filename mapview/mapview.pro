TEMPLATE	= lib

TARGET		= mapview

CONFIG		+= qt warn_on release

DESTDIR = ../build

QT += sql

INCLUDEPATH += ./ \
              ../my_mapaccess \
              ../include
			   
DEPENDPATH += 	. \
              ../my_mapaccess

LIBPATH   += ../build

LIBS += -L../build -lmy_mapaccess


DEFINES += __USE_DLL_XXXXX
DEFINES     += BUILD_DLL

include(mapview.pri)
