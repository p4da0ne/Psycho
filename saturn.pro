TEMPLATE = subdirs

CONFIG += ordered

QT   += gui widgets sql

# Directories
SUBDIRS += \
        my_mapaccess \
		coord \
        manage_users \
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
		mapview \
		mainform \

	

INSTALLS += transl
TRANSLATIONS = saturn_ru.ts

