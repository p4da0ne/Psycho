TEMPLATE	= lib

TARGET		= mapview

DESTDIR = ../build

QT += sql

RESOURCES       += mapview.qrc

INCLUDEPATH += ./ \
				../include \
				../coord \
				../my_mapaccess \
			   ./calculatingClasses \
			   ../reports \
			   ../searchengine
			   

			   
DEPENDPATH += 	. \
				../coord \
				../my_mapaccess \
				../reports \
				../searchengine

LIBPATH   += ../build

LIBS += -lcoord -lmy_mapaccess -lreports -lsearchengine


DEFINES += __USE_DLL_XXXXX
DEFINES     += BUILD_DLL

include(mapview.pri)