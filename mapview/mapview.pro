TEMPLATE	= lib

TARGET		= mapview

CONFIG		+= qt warn_on

DESTDIR = ../build

QT += gui sql widgets core5compat printsupport

RESOURCES       += mapview.qrc

INCLUDEPATH += ./ \
				../include \
				../coord \
				../my_mapaccess \
			   ./calculatingClasses \
			   ../reports \
			   ../searchengine \
			   ../eventsmodel \
			   ../event_manager
			   

			   
DEPENDPATH += 	. \
				../coord \
				../my_mapaccess \
				../reports \
				../searchengine \
				 ../eventsmodel \
				 ../event_manager

LIBPATH   += ../build

LIBS += -lcoord -lmy_mapaccess -lreports -lsearchengine -leventsmodel -levent_manager



DEFINES += __USE_DLL_XXXXX
DEFINES     += BUILD_DLL

include(mapview.pri)
