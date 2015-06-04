TEMPLATE	= lib

TARGET		= mapview

CONFIG		+= qt warn_on

DESTDIR = ../build

QT += sql

RESOURCES       += mapview.qrc

INCLUDEPATH += ./ \
				../include \
				../coord \
				../my_mapaccess \
			   ./calculatingClasses \
			   ../reports \
			   ../searchengine \
			   ../eventsmodel
			   

			   
DEPENDPATH += 	. \
				../coord \
				../my_mapaccess \
				../reports \
				../searchengine \
				 ../eventsmodel

LIBPATH   += ../build

LIBS += -lcoord -lmy_mapaccess -lreports -lsearchengine -leventsmodel



DEFINES += __USE_DLL_XXXXX
DEFINES     += BUILD_DLL

include(mapview.pri)