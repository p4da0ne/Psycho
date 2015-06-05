TEMPLATE        = app
TARGET          = saturn
DESTDIR         = ../build

QT              += sql network

RESOURCES       += saturn.qrc

DEPENDPATH += ../my_mapaccess \
              ../dataaccess \
              ../db_saturn \
			  ../users_manager \
              ../supporting_tables \
              ../object_manager \
              ../mapview \
              ../output \
              ../reports \
              ../signs_edit \
			  ../event_manager

include(mainform.pri)


LIBS += -L../build -lmy_mapaccess \
					-ldataaccess \
					-ldb_saturn \
					-lsupporting_tables \
					-lmanage_users \
					-lusers_manager \
					-lmapview \
					-loutput \
					-lreports \
					-lobject_manager \
					-lsigns_edit \
					-levent_manager
					
DEFINES     += BUILD_DLL

win32 {
INCLUDEPATH += . \
        ../include \
        ../my_mapaccess \
        ../dataaccess \
	../db_saturn \
        ../manage_users \
		../users_manager \
	../supporting_tables \
	../object_manager \
        ../mapview \
        ../output \
        ../reports \
        ../signs_edit \
		../event_manager
}

