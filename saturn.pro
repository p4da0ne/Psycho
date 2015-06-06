TEMPLATE = subdirs

CONFIG += ordered

QT   += gui widgets sql

# Directories
SUBDIRS += \
        my_mapaccess \
		coord \
		users_manager \
        output \
		textedit \
        reports \
        dataaccess \
        db_saturn \
        supporting_tables \
        object_manager \
        signs_edit \
        eventsmodel \
		searchengine \
		mapview \
		event_manager \
		mainform \

	

INSTALLS += transl
TRANSLATIONS = saturn_ru.ts

