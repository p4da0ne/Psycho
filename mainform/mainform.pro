TEMPLATE        = app
TARGET          = saturn
DESTDIR         = ../build

QT              += sql network

DEPENDPATH += ../my_mapaccess \
              ../dataaccess \
              ../db_saturn \
              ../supporting_tables \
              ../object_manager \
              ../mapview \
              ../output \
              ../reports

include(mainform.pri)


LIBS += -L../build -lmy_mapaccess -ldataaccess -ldb_saturn -lsupporting_tables -lmanage_users -lmapview  -loutput -lreports -lobject_manager
DEFINES     += BUILD_DLL

win32 {
INCLUDEPATH += . \
        ../include \
        ../my_mapaccess \
        ../dataaccess \
	../db_saturn \
        ../manage_users \
	../supporting_tables \
	../object_manager \
        ../mapview \
        ../output \
        ../reports
}

