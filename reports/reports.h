#ifndef REPORTS_H
#define REPORTS_H

#include "config_reports.h"

#include <QObject>


class __EXPORT_REPORTS Reports : public QObject
{
    Q_OBJECT
public:
    /// Default constructor
    explicit Reports(QObject *parent=0);
    /// Destructor
    ~Reports();


	 QString create_object_formular(int id_object = 0);
     QString create_object_formular_pers(int id_object = 0);
     QString create_object_formular_smi(int id_object = 0);
     QString create_object_formular_ls(int id_object = 0);
	 void show_preview_dialog(QString html);
     void show_preview_dialog1(QString html);
     void show_preview_dialog2(QString html);
	

private:

    
};

#endif // REPORTS_H
