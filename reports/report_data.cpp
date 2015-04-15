#include "report_data.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QStringList>

ReportData::ReportData(QObject *parent)
    : QObject(parent)
{
  
    
}

ReportData::~ReportData()
{
    
}

//========== Получение данных об объекте по его id ===========

QMap <int, QMap< QString,QString> > ReportData::get_obj_info(int id_object)
{
  //  obj_info = new QMap<QString, QString>;
    obj_info = new QMap <int, QMap< QString,QString> >;
    obj_info->clear();
    QSqlQuery query;
    query.prepare ("SELECT gr.name_groups,gr.counte_groups,gr.founder_group,gr.menegement_groups,gr.officce_groups,gr.description_groups,gr.propaganda_groups, tr.name_trend_groups,sph.name_sphere_groups, form.name_form_groups, reg.name_region, coord.x_coordinates,coord.y_coordinates \
            FROM   groups gr,trend_groups tr,sphere_groups sph, form_groups form, region reg, coordinates coord \
            WHERE  (id_groups = ?) and (gr.id_trend=tr.id_trend_groups)and \
                   (gr.id_sphere_groups=sph.id_sphere_groups) and (gr.id_form_groups=form.id_form_groups) and \
                  (gr.id_region = reg.id_region) and (gr.id_coordinates = coord.id_coordinates)");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return *obj_info;
    }

      QSqlRecord rec = query.record();
    QMap<QString, QString> map;
    query.next();
        map.clear();
        map.insert("1. Наименование региона",query.value(rec.indexOf("name_region")).toString());
        obj_info->insert(1,map);
        map.clear();
        map.insert("2. Наименование организации",query.value(rec.indexOf("name_groups")).toString());
        obj_info->insert(2,map);
        map.clear();
        map.insert("3. Руководитель организации",query.value(rec.indexOf("founder_group")).toString());
        obj_info->insert(3,map);
        map.clear();
        map.insert("4. Численность организации",query.value(rec.indexOf("counte_groups")).toString());
        obj_info->insert(4,map);
        map.clear();
        map.insert("5. Описание организации",query.value(rec.indexOf("description_groups")).toString());
        obj_info->insert(5,map);
        map.clear();
        map.insert("6. menegement_groups",query.value(rec.indexOf("menegement_groups")).toString());
        obj_info->insert(6,map);
        map.clear();
        map.insert("7. officce_groups",query.value(rec.indexOf("officce_groups")).toString());
        obj_info->insert(7,map);
        map.clear();
        map.insert("8. propaganda_groups",query.value(rec.indexOf("propaganda_groups")).toString());
        obj_info->insert(8,map);
        map.clear();
        map.insert("9. name_trend_groups",query.value(rec.indexOf("name_trend_groups")).toString());
        obj_info->insert(9,map);
        map.clear();
        map.insert("10. name_sphere_groups",query.value(rec.indexOf("name_sphere_groups")).toString());
        obj_info->insert(10,map);
        map.clear();
        map.insert("11. name_form_groups",query.value(rec.indexOf("name_form_groups")).toString());
        obj_info->insert(11,map);
        map.clear();
        QString w ="X= "+ query.value(rec.indexOf("x_coordinates")).toString()+";  Y= "+query.value(rec.indexOf("y_coordinates")).toString();
        map.insert("12. Координаты объекта",w);
        obj_info->insert(12,map);

   return *obj_info;

}

QMap<QString, QString> ReportData::pers_info(int id_object)
{
    pers_info_date = new QMap<QString, QString>;
    pers_info_date->clear();
    QSqlQuery query;
    query.prepare ("SELECT pers.name_persones,pers.age_persones,pers.contact_persones, pers.description_persones, pers.authority_persones, pers.opposition_persones, pers.rank_persones, image_persones \
            FROM   persones pers  \
            WHERE  id_persones = ?");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return *pers_info_date;
    }
    QMap<QString, QString> map;
    QSqlRecord rec = query.record();

    QString foto_name;

    query.next();
       foto_name = query.value(rec.indexOf("image_persones")).toString();
       QString path_foto = "foto_persones/" + foto_name;
       pers_info_date->insert("1. Фотография:","<CENTER><IMG BORDER=\"0\" SRC=\""+path_foto+"\" width = '200' height = '200'> </CENTER>");
       pers_info_date->insert("2. Фамилия, Имя, Отчество",query.value(rec.indexOf("name_persones")).toString());
       pers_info_date->insert("3. Должность(звание)",query.value(rec.indexOf("rank_persones")).toString());
       pers_info_date->insert("4. Контакты",query.value(rec.indexOf("contact_persones")).toString());
       pers_info_date->insert("5. Характеристика",query.value(rec.indexOf("description_persones")).toString());
       pers_info_date->insert("6. Авторитет",query.value(rec.indexOf("authority_persones")).toString());
       pers_info_date->insert("7. Оппозиционность",query.value(rec.indexOf("opposition_persones")).toString());


       return *pers_info_date;

}

QMap<int, QMap<QString, QString> > ReportData::smi_info(int id_object)
{
    smi_info_date = new QMap<int, QMap<QString, QString> >;
    smi_info_date->clear();


    QSqlQuery query;
    query.prepare ("SELECT DISTINCT sm.name_smi,sm.power_broadcast_smi,sm.site_smi, sm.description_smi, sm.frequency_smi, sm.auditoria, sm.tirag, tsmi.nametype_smi, tosmi.name_type_office_smi, tbsmi.name_type_broadcast_smi, possmi.name_position_smi, lsmi.name_level_smi, thsmi.name_theme_smi,reg.name_region \
                   FROM   smi sm, type_smi tsmi, type_office_smi tosmi, type_broadcast_smi tbsmi, position_smi possmi, level_smi lsmi, theme_smi thsmi,region reg \
                   WHERE  (id_smi = ?) \
                               and (sm.id_type_smi=tsmi.id_type_smi) \
                               and (sm.id_type_office_smi=tosmi.id_type_office_smi)\
                               and (sm.id_type_broadcast_smi=tbsmi.id_type_broadcast_smi)\
                               and (sm.id_position_smi=possmi.id_position_smi) \
                               and (sm.id_level_smi=lsmi.id_level_smi) \
                               and (sm.id_theme_smi=thsmi.id_theme_smi)");
//                               and reg.name_region = (SELECT reg.name_region \
//                               FROM smi_region srd \
//                               WHERE (srd.id_smi = ?) and (srd.id_region=reg.id_region))");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return *smi_info_date;
    }

    QMap<QString, QString> map;
    QSqlRecord rec = query.record();
    query.next();
//        map.clear();
//        map.insert("1. Регион СМИ",query.value(rec.indexOf("name_region")).toString());
//        smi_info_date->insert(1,map);
        map.clear();
        map.insert("1. Наименование СМИ",query.value(rec.indexOf("name_smi")).toString());
        smi_info_date->insert(1,map);
        map.clear();
        map.insert("2. Радиус действия",query.value(rec.indexOf("power_broadcast_smi")).toString());
        smi_info_date->insert(2,map);
        map.clear();
        map.insert("3. Сайт",query.value(rec.indexOf("site_smi")).toString());
        smi_info_date->insert(3,map);
        map.clear();
        map.insert("4. Описание СМИ",query.value(rec.indexOf("description_smi")).toString());
        smi_info_date->insert(4,map);
        map.clear();
        map.insert("5. Частота вещания",query.value(rec.indexOf("frequency_smi")).toString());
        smi_info_date->insert(5,map);
        map.clear();
        map.insert("6. Аудитория",query.value(rec.indexOf("auditoria")).toString());
        smi_info_date->insert(6,map);
        map.clear();
        map.insert("7. Тираж",query.value(rec.indexOf("tirag")).toString());
        smi_info_date->insert(7,map);
        map.clear();
        map.insert("8. Тип СМИ",query.value(rec.indexOf("nametype_smi")).toString());
        smi_info_date->insert(8,map);
        map.clear();
        map.insert("9. Представительство",query.value(rec.indexOf("name_type_office_smi")).toString());
        smi_info_date->insert(9,map);
        map.clear();
        map.insert("10. Способ вещания",query.value(rec.indexOf("name_type_broadcast_smi")).toString());
        smi_info_date->insert(10,map);
        map.clear();
        map.insert("11. Направленность",query.value(rec.indexOf("name_position_smi")).toString());
        smi_info_date->insert(11,map);
        map.clear();
        map.insert("12. Уровень вещания",query.value(rec.indexOf("name_level_smi")).toString());
        smi_info_date->insert(12,map);
        map.clear();
        map.insert("13. Тематика",query.value(rec.indexOf("name_theme_smi")).toString());
        smi_info_date->insert(13,map);

        return *smi_info_date;

}

QMap<int, QMap<QString, QString> > ReportData::ls_info(int id_object)
{
    ls_info_date = new QMap<int, QMap<QString, QString> >;
    ls_info_date->clear();


    QSqlQuery query;
    query.prepare ("SELECT l.name_ls, l.enimy_ls, l.mpo_ls, l.counte_ls, l.counte_ls_bd, l.mps_priz_ls, l.mps_konrt_ls, l.mps_of_ls, l.short_name_ls, reg.name_region, tls.name_type_ls, coord.x_coordinates, coord.y_coordinates \
                   FROM   ls l, region reg, type_ls tls, coordinates coord \
                   WHERE  (id_ls = ?) and (l.id_type_ls = tls.id_type_ls) and (l.id_coordinates = coord.id_coordinates) \
                                        and (l.id_region = reg.id_region)");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return *ls_info_date;
    }

    QMap<QString, QString> map;
    QSqlRecord rec = query.record();
    query.next();
        map.clear();
        map.insert("1. Наименование воиснкого формирования",query.value(rec.indexOf("name_ls")).toString());
        ls_info_date->insert(1,map);
        map.clear();
        map.insert("2. Дислокация",query.value(rec.indexOf("name_region")).toString());
        ls_info_date->insert(2,map);
        map.clear();
        map.insert("3. Враждебность",query.value(rec.indexOf("enimy_ls")).toString());
        ls_info_date->insert(3,map);
        map.clear();
        map.insert("4. Подразделение МПО",query.value(rec.indexOf("mpo_ls")).toString());
        ls_info_date->insert(4,map);
        map.clear();
        map.insert("5. Численность личного состава",query.value(rec.indexOf("counte_ls")).toString());
        ls_info_date->insert(5,map);
        map.clear();
        map.insert("6. Количество учавствующих в боевых действиях",query.value(rec.indexOf("counte_ls_bd")).toString());
        ls_info_date->insert(6,map);
        map.clear();
        map.insert("7. МПС по призыву",query.value(rec.indexOf("mps_priz_ls")).toString());
        ls_info_date->insert(7,map);
        map.clear();
        map.insert("8. МПС по контракту",query.value(rec.indexOf("mps_konrt_ls")).toString());
        ls_info_date->insert(8,map);
        map.clear();
        map.insert("9. МПС офицерского состава",query.value(rec.indexOf("mps_of_ls")).toString());
        ls_info_date->insert(9,map);
        map.clear();
        map.insert("10. Краткое наименование формирования",query.value(rec.indexOf("short_name_ls")).toString());
        ls_info_date->insert(10,map);
        map.clear();
        /*map.insert("11. name_type_ls",query.value(rec.indexOf("name_type_ls")).toString());
        ls_info_date->insert(11,map);*/

        map.clear();
        QString w ="X= "+ query.value(rec.indexOf("x_coordinates")).toString()+";  Y= "+query.value(rec.indexOf("y_coordinates")).toString();
        map.insert("12. Координаты формирования",w);
        ls_info_date->insert(12,map);

        return *ls_info_date;
}

