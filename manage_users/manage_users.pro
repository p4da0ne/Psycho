TEMPLATE = lib
TARGET = manage_users

QT	+= sql

DESTDIR = ../build

INCLUDEPATH += . \

DEFINES += __USE_DLL_XXXXX

include(manage_users.pri)