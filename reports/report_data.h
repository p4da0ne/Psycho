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
    QMap<QString, QMap<QString, QString> > obj_info_coord(int id_object=0);
    QMap<QString,QString> pers_info(int id_object = 0);
    QMap<QString, QMap<QString,QString> > pers_info_coord (int id_object = 0);
    QMap<int, QMap<QString, QString> > smi_info(int id_object = 0);
    QMap<int, QMap<QString, QString> > ls_info(int id_object = 0);
    QMap<int, QMap<QString, QString> > sc_info(int id_object = 0);
    QMap<QString, QMap<QString, QString> > sc_info_coord(int id_object=0);
    QMap<QString, QMap<QString, QString> > ls_info_coord(int id_object=0);
    QMap<int, QMap<QString, QString> > mpo_pso_smi_info(int id_object=0);
    QMap<QString, QMap<QString, QString> > mpo_pso_info_coord(int id_object=0);
    QMap<int, QMap<QString, QString> > mpo_pso_ls_info(int id_object=0);
    QMap<int, QMap<QString, QString> > mpo_pso_gr_info(int id_object=0);
    QString get_pers_foto_from_DB(int);
    QString get_mpo_pso_foto_from_DB(int id_mpo);
    QString get_sc_foto_from_DB(int);
//	QMultiMap<QString,QString> get_obj_v(int id_object = 0);
   
private:

    QMap <int, QMap< QString,QString> > *obj_info;
    QMap<QString, QString> *pers_info_date;
    QMap<QString,QMap<QString,QString> > *obj_elem;
    QMap<QString,QMap<QString,QString> > *obj_elem_obj;
    QMap <int, QMap< QString,QString> > *smi_info_date;
    QMap <int, QMap< QString,QString> > *ls_info_date;
    QMap <int, QMap< QString,QString> > *sc_info_date;
    QMap<QString,QMap<QString,QString> > *ls_elem_obj;
    QMap<QString,QMap<QString,QString> > *mpo_elem_obj;
    QMap<QString,QMap<QString,QString> > *sc_elem_obj;
    QMap <int, QMap< QString,QString> > *mpsmi_info_date;
    QMap <int, QMap< QString,QString> > *mpls_info_date;
    QMap <int, QMap< QString,QString> > *mpgr_info_date;
//	QMultiMap<QString,QString> *obj_info_v;
};

#endif // REPORT_DATA_H
