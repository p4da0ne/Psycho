TEMPLATE	= lib

TARGET		= mapview

DESTDIR = ../build

QT += sql

INCLUDEPATH += ./ \
				../include \
			   ../my_mapaccess \
			   ./calculatingClasses
			   

			   
DEPENDPATH += 	. \
				../my_mapaccess

LIBPATH   += ../build

LIBS += -lmy_mapaccess


DEFINES += __USE_DLL_XXXXX
DEFINES     += BUILD_DLL

include(mapview.pri)