#ifndef REPORT_DATA_H
#define REPORT_DATA_H

#include <QObject>
#include <QMap>


class ReportData : public QObject
{
    Q_OBJECT
public:
    /// Default constructor
    explicit ReportData(QObject *parent=0);
    /// Destructor
    ~ReportData();
	 
	QMap<QString,QString> get_obj_info(int id_object = 0);
	QMultiMap<QString,QString> get_obj_v(int id_object = 0);
   
private:
	QMap<QString, QString> *obj_info;
	QMultiMap<QString,QString> *obj_info_v;
};

#endif // REPORT_DATA_H
