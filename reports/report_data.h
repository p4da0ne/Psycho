#ifndef REPORT_DATA_H
#define REPORT_DATA_H

#include <QObject>
#include <QSettings>
#include <QMap>


class ReportData : public QObject
{
    Q_OBJECT
public:
    /// Default constructor
    explicit ReportData(QObject *parent=0);
    /// Destructor
    ~ReportData();

    QMap<int, QMap<QString, QString> > get_obj_info(int id_object = 0);
    QMap<QString,QString> pers_info(int id_object = 0);
    QMap<int, QMap<QString, QString> > smi_info(int id_object = 0);
    QMap<int, QMap<QString, QString> > ls_info(int id_object = 0);
//	QMultiMap<QString,QString> get_obj_v(int id_object = 0);
   
private:

    QMap <int, QMap< QString,QString> > *obj_info;
    QMap<QString, QString> *pers_info_date;
    QMap <int, QMap< QString,QString> > *smi_info_date;
    QMap <int, QMap< QString,QString> > *ls_info_date;
//	QMultiMap<QString,QString> *obj_info_v;
};

#endif // REPORT_DATA_H
