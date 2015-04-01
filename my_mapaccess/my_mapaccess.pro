TEMPLATE	= lib

CONFIG		+= qt warn_on dll
			
TARGET		= my_mapaccess

DESTDIR = ../build


DEFINES += __USE_DLL_MY_MAPACCESS
DEFINES += BUILD_DLL

INCLUDEPATH += ./ \
			   ../include


include(my_mapaccess.pri)