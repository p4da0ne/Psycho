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
	 void show_preview_dialog(QString html);
	

private:

    
};

#endif // REPORTS_H
