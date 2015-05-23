TEMPLATE	= lib

TARGET		= mapview

DESTDIR = ../build

QT += sql

RESOURCES       += mapview.qrc

INCLUDEPATH += ./ \
				../include \
			   ../my_mapaccess \
			   ./calculatingClasses \
			   ../reports
			   

			   
DEPENDPATH += 	. \
				../my_mapaccess \
				../reports

LIBPATH   += ../build

LIBS += -lmy_mapaccess -lreports


DEFINES += __USE_DLL_XXXXX
DEFINES     += BUILD_DLL

include(mapview.pri)