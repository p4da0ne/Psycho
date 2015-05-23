TEMPLATE = subdirs

CONFIG += ordered

QT   += gui widgets sql

# Directories
SUBDIRS += \
        my_mapaccess \
        manage_users \
        mapview \
        output \
        reports \
        dataaccess \
        db_saturn \
        supporting_tables \
        object_manager \
        mainform \
        signs_edit \
        textedit \
                eventsmodel
	

INSTALLS += transl
TRANSLATIONS = saturn_ru.ts

