#ifndef REPORT_DATA_H
#define REPORT_DATA_H

#include <QObject>
#include <QSettings>
#include <QMap>
#include <QStringList>


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
    QMap<int, QMap<QString, QString> > region_info(int id_object = 0);
    QMap<int, QMap<QString, QString> > region_info_pop(int id_object = 0);
    QMap<int, QMap<QString, QString> > region_info_smi(int id_object = 0);
    QMap<int, QMap<QString, QString> > region_info_group(int id_object = 0);
    QMap<QString, QString> region_info_factor(int id_object = 0);
    QMap<QString, QString> region_info_itog(float rez_ = 0);
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
    QMap <int, QMap< QString,QString> > *region_info_date;
    QMap <int, QMap< QString,QString> > *reg_info_date;
    QMap <int, QMap< QString,QString> > *sc_info_date;
    QMap<QString,QMap<QString,QString> > *ls_elem_obj;
    QMap<QString,QMap<QString,QString> > *mpo_elem_obj;
    QMap<QString,QMap<QString,QString> > *sc_elem_obj;
    QMap <int, QMap< QString,QString> > *mpsmi_info_date;
    QMap <int, QMap< QString,QString> > *mpls_info_date;
    QMap <int, QMap< QString,QString> > *mpgr_info_date;
    QMap<QString,QString> *info_date;
    QString name_nations_string;
    QString name_age_string;
    QString persent_age_string;
    QString name_confessions;
    QString persent_confessions;
    int number_m,number_w,smi_number,number_nations;
    float number_sex_m,number_sex_w,number,persent_nations,rez_z_1;
    double factori[33];
    QStringList factor_text;
    int factorflag;
//	QMultiMap<QString,QString> *obj_info_v;
};

#endif // REPORT_DATA_H
