TEMPLATE = lib
TARGET = supporting_tables

DESTDIR = ../build

QT              += sql network widgets gui core5compat

DEPENDPATH +=   ../my_mapaccess \
                ../dataaccess \
                ../db_saturn

include(supporting_tables.pri)



LIBS += -L../build -lmy_mapaccess -ldataaccess -ldb_saturn

DEFINES += __USE_DLL_XXXXX
#DEFINES     += BUILD_DLL


win32 {
INCLUDEPATH += ../include \
                ../my_mapaccess \
                ../dataaccess \
                ../db_saturn
}



