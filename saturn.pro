TEMPLATE = subdirs

CONFIG += debug ordered

QT   += gui core

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
        calculatingproblemmanager \
        mainform
	

INSTALLS += transl
TRANSLATIONS = saturn_ru.ts
