TEMPLATE        = lib
TARGET          = textedit
DESTDIR = ../build
CONFIG          += qt warn_on

LIBS += -lqaxcontainer


build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}
DEFINES += __USE_DLL_TEXTEDIT
DEFINES += BUILD_DLL
include(textedit.pri)
# install
#target.path = ./textedit
#sources.files = $$SOURCES $$HEADERS $$RESOURCES *.pro *.html *.doc images
#sources.path = ./textedit
#INSTALLS += target sources
