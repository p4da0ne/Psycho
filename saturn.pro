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
        signs_edit \
        textedit \
        eventsmodel \
		searchengine \
		mainform \

	

INSTALLS += transl
TRANSLATIONS = saturn_ru.ts

