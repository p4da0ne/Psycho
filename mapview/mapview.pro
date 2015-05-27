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
			   ../searchengine \
			   ../eventsmodel
			   

			   
DEPENDPATH += 	. \
				../my_mapaccess \
				../reports \
				../searchengine \
				 ../eventsmodel

LIBPATH   += ../build

LIBS += -lmy_mapaccess -lreports -lsearchengine -leventsmodel


DEFINES += __USE_DLL_XXXXX
DEFINES     += BUILD_DLL

include(mapview.pri)