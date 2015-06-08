#object_manager

HEADERS += \
         config_object_manager.h \
         object_manager.h \
         add_object.h \
         add_elements_dialog.h \
         simledelegate.h \
         Calculate_K_omkrf.h \
         calculating_mps.h \
         textprinter.h \
         People_Losses.h \
         mylabel.h \
         mysqlrelationaldelegate.h \
         personesdata.h \
        persones_info.h \
    comboboxdelegat.h

#Source files
SOURCES +=  object_manager.cpp \
        add_object.cpp \
        add_elements_dialog.cpp \
        simledelegate.cpp \
        Calculate_K_omkrf.cpp \
        calculating_mps.cpp \
        textprinter.cpp \
        People_Losses.cpp \
        mylabel.cpp \
        mysqlrelationaldelegate.cpp \
        personesdata.cpp \
        persones_info.cpp \
        comboboxdelegat.cpp

		
#Forms
FORMS += object_manager_form.ui\
         add_object.ui \
    personesdata_form.ui \
    persones_info_form.ui
