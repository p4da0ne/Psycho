TEMPLATE	= lib

TARGET		= mapview

DESTDIR = ../build

QT += sql

RESOURCES       += mapview.qrc

INCLUDEPATH += ./ \
				../include \
			   ../my_mapaccess \
			   ./calculatingClasses \
			   ../reports \
			   ../searchengine
			   

			   
DEPENDPATH += 	. \
				../my_mapaccess \
				../reports \
				../searchengine

LIBPATH   += ../build

LIBS += -lmy_mapaccess -lreports -lsearchengine


DEFINES += __USE_DLL_XXXXX
DEFINES     += BUILD_DLL

include(mapview.pri)