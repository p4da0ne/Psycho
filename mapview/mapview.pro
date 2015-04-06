TEMPLATE	= lib

TARGET		= mapview

CONFIG		+= qt warn_on

DESTDIR = ../build

QT += sql gui core

INCLUDEPATH += ./ \
              ../my_mapaccess \
              ../include
			   
DEPENDPATH += 	. \
              ../my_mapaccess

LIBPATH   += ../build

LIBS += -L../build -lmy_mapaccess


DEFINES += __USE_DLL_MAPVIEW
DEFINES     += BUILD_DLL

include(mapview.pri)
