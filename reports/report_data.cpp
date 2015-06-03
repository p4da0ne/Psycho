#include "report_data.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QStringList>
#include <QPixmap>
#include <QDir>

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
    query.prepare("SELECT gr.name_groups,gr.counte_groups,gr.founder_group,gr.menegement_groups,gr.officce_groups,gr.description_groups,gr.propaganda_groups,tr.name_trend_groups,sph.name_sphere_groups,form.name_form_groups,reg.name_region \
                  FROM groups gr,trend_groups tr,sphere_groups sph, form_groups form, region reg \
                  WHERE (gr.id_groups = ?) and (gr.id_trend=tr.id_trend_groups) and (gr.id_sphere_groups=sph.id_sphere_groups) and (gr.id_form_groups=form.id_form_groups) and (gr.id_region = reg.id_region)");

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
        map.insert("1. Наименование региона:",query.value(rec.indexOf("name_region")).toString());
        obj_info->insert(1,map);
        map.clear();
        map.insert("2. Наименование организации:",query.value(rec.indexOf("name_groups")).toString());
        obj_info->insert(2,map);
        map.clear();
        map.insert("3. Руководитель организации:",query.value(rec.indexOf("founder_group")).toString());
        obj_info->insert(3,map);
        map.clear();
        map.insert("4. Численность организации:",query.value(rec.indexOf("counte_groups")).toString());
        obj_info->insert(4,map);
        map.clear();
        map.insert("5. Описание организации:",query.value(rec.indexOf("description_groups")).toString());
        obj_info->insert(5,map);
        map.clear();
        map.insert("6. Руководство организации:",query.value(rec.indexOf("menegement_groups")).toString());
        obj_info->insert(6,map);
        map.clear();
        map.insert("7. Представительство:",query.value(rec.indexOf("officce_groups")).toString());
        obj_info->insert(7,map);
        map.clear();
        map.insert("8. Информационные органы организации:",query.value(rec.indexOf("propaganda_groups")).toString());
        obj_info->insert(8,map);
        map.clear();
        map.insert("9. Направленость организации:",query.value(rec.indexOf("name_trend_groups")).toString());
        obj_info->insert(9,map);
        map.clear();
        map.insert("10. Сфера деятельности:",query.value(rec.indexOf("name_sphere_groups")).toString());
        obj_info->insert(10,map);
        map.clear();
        map.insert("11. Форма организации:",query.value(rec.indexOf("name_form_groups")).toString());
        obj_info->insert(11,map);
        map.clear();

   return *obj_info;

}
QMap<QString, QMap<QString, QString> > ReportData::obj_info_coord(int id_object)
{
           QString name_obj;
           obj_elem_obj = new QMap <QString,QMap<QString,QString> >;
           obj_elem_obj->clear();
           QSqlQuery query;

           QString str = "SELECT *\
                          FROM    coord_groups gr, coordinates coord\
                          WHERE  (gr.id_groups = ?)and \
                          (gr.id_coordinates=coord.id_coordinates)";
           query.prepare(str);
           query.addBindValue(id_object);
               if(!query.exec())
               {
                   QString sss = query.lastError().text();
                   return *obj_elem_obj;
               }
           QSqlRecord rec = query.record();

           QMap<QString, QString> map;
           query.next();
           name_obj="12. Координаты организации:";
                       map.clear();
                       QString w = "X= ";
                       w.append(query.value(rec.indexOf("x_coordinates")).toString());
                       w.append("<br>Y= ");
                       w.append(query.value(rec.indexOf("y_coordinates")).toString());
                       map.insert("Прямоугольные координаты",w);

                       QString q = "Широта: ";
                       q.append(query.value(rec.indexOf("latitude_wgs_84_g")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("latitude_wgs_84_m")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("latitude_wgs_84_s")).toString());
                       q.append("<br>Долгота: ");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_g")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_m")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_s")).toString());

                       map.insert("Геодезические координаты",q);
                       obj_elem_obj->insert(name_obj,map);

            return *obj_elem_obj;
  }
QMap<QString, QString> ReportData::pers_info(int id_object)
{
    pers_info_date = new QMap<QString, QString>;
    pers_info_date->clear();
    QSqlQuery query;
    query.prepare ("SELECT pers.name_persones,pers.age_persones,pers.contact_persones, pers.description_persones, pers.authority_persones, pers.opposition_persones, pers.rank_persones, type_persones.name_type_persones FROM  persones pers, type_persones WHERE pers.id_persones = ? AND pers.id_type_persones = type_persones.id_type_persones ");

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
    foto_name = get_pers_foto_from_DB(id_object);

       pers_info_date->insert("1. Фотография:","<CENTER><IMG BORDER=\"0\" SRC=\""+foto_name+"\" height=200> </CENTER>");
       pers_info_date->insert("2. Фамилия, Имя, Отчество:",query.value(rec.indexOf("name_persones")).toString());
       pers_info_date->insert("3. Возраст:",query.value(rec.indexOf("age_persones")).toString());
       pers_info_date->insert("4. Должность(звание):",query.value(rec.indexOf("rank_persones")).toString());
       pers_info_date->insert("5. Контакты:",query.value(rec.indexOf("contact_persones")).toString());
       pers_info_date->insert("6. Характеристика:",query.value(rec.indexOf("description_persones")).toString());
       pers_info_date->insert("7. Авторитет:",query.value(rec.indexOf("authority_persones")).toString());
       pers_info_date->insert("8. Оппозиционность:",query.value(rec.indexOf("opposition_persones")).toString());
       pers_info_date->insert("9. Тип персоналии:",query.value(rec.indexOf("name_type_persones")).toString());

       return *pers_info_date;
}

QMap<QString, QMap<QString, QString> > ReportData::pers_info_coord(int id_object)
{
           QString name_obj;
           obj_elem = new QMap <QString,QMap<QString,QString> >;
           obj_elem->clear();
           QSqlQuery query;

           QString str = "SELECT *\
                          FROM    coord_persones pc, coordinates coord\
                          WHERE  (pc.id_persones = ?)and \
                          (pc.id_coordinates=coord.id_coordinates)";
           query.prepare(str);
           query.addBindValue(id_object);
               if(!query.exec())
               {
                   QString sss = query.lastError().text();
                   return *obj_elem;
               }
           QSqlRecord rec = query.record();

           QMap<QString, QString> map;
           query.next();
           name_obj="10. Координаты персоны:";
                       map.clear();
                       QString w = "X= ";
                       w.append(query.value(rec.indexOf("x_coordinates")).toString());
                       w.append("<br>Y= ");
                       w.append(query.value(rec.indexOf("y_coordinates")).toString());
                       map.insert("Прямоугольные координаты",w);

                       QString q = "Широта: ";
                       q.append(query.value(rec.indexOf("latitude_wgs_84_g")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("latitude_wgs_84_m")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("latitude_wgs_84_s")).toString());
                       q.append("<br>Долгота: ");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_g")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_m")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_s")).toString());

                       map.insert("Геодезические координаты",q);
                       obj_elem->insert(name_obj,map);

            return *obj_elem;
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
        map.insert("1. Наименование СМИ:",query.value(rec.indexOf("name_smi")).toString());
        smi_info_date->insert(1,map);
        map.clear();
        map.insert("2. Радиус действия:",query.value(rec.indexOf("power_broadcast_smi")).toString());
        smi_info_date->insert(2,map);
        map.clear();
        map.insert("3. Сайт:",query.value(rec.indexOf("site_smi")).toString());
        smi_info_date->insert(3,map);
        map.clear();
        map.insert("4. Описание СМИ:",query.value(rec.indexOf("description_smi")).toString());
        smi_info_date->insert(4,map);
        map.clear();
        map.insert("5. Частота вещания:",query.value(rec.indexOf("frequency_smi")).toString());
        smi_info_date->insert(5,map);
        map.clear();
        map.insert("6. Аудитория:",query.value(rec.indexOf("auditoria")).toString());
        smi_info_date->insert(6,map);
        map.clear();
        map.insert("7. Тираж:",query.value(rec.indexOf("tirag")).toString());
        smi_info_date->insert(7,map);
        map.clear();
        map.insert("8. Тип СМИ:",query.value(rec.indexOf("nametype_smi")).toString());
        smi_info_date->insert(8,map);
        map.clear();
        map.insert("9. Представительство:",query.value(rec.indexOf("name_type_office_smi")).toString());
        smi_info_date->insert(9,map);
        map.clear();
        map.insert("10. Способ вещания:",query.value(rec.indexOf("name_type_broadcast_smi")).toString());
        smi_info_date->insert(10,map);
        map.clear();
        map.insert("11. Направленность:",query.value(rec.indexOf("name_position_smi")).toString());
        smi_info_date->insert(11,map);
        map.clear();
        map.insert("12. Уровень вещания:",query.value(rec.indexOf("name_level_smi")).toString());
        smi_info_date->insert(12,map);
        map.clear();
        map.insert("13. Тематика:",query.value(rec.indexOf("name_theme_smi")).toString());
        smi_info_date->insert(13,map);

        return *smi_info_date;

}

QMap<int, QMap<QString, QString> > ReportData::ls_info(int id_object)
{
    ls_info_date = new QMap<int, QMap<QString, QString> >;
    ls_info_date->clear();


    QSqlQuery query;
    query.prepare ("SELECT l.name_ls, l.enimy_ls, l.mpo_ls, l.counte_ls, l.counte_ls_bd, l.mps_priz_ls, l.mps_konrt_ls, l.mps_of_ls, l.short_name_ls, reg.name_region, tls.name_type_ls \
                   FROM   ls l, region reg, type_ls tls \
                   WHERE  (id_ls = ?) and (l.id_type_ls = tls.id_type_ls) \
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
        map.insert("1. Наименование воиснкого формирования:",query.value(rec.indexOf("name_ls")).toString());
        ls_info_date->insert(1,map);
        map.clear();
        map.insert("2. Дислокация:",query.value(rec.indexOf("name_region")).toString());
        ls_info_date->insert(2,map);
        map.clear();
        map.insert("3. Враждебность:",query.value(rec.indexOf("enimy_ls")).toString());
        ls_info_date->insert(3,map);
        map.clear();
        map.insert("4. Подразделение МПО:",query.value(rec.indexOf("mpo_ls")).toString());
        ls_info_date->insert(4,map);
        map.clear();
        map.insert("5. Численность личного состава:",query.value(rec.indexOf("counte_ls")).toString());
        ls_info_date->insert(5,map);
        map.clear();
        map.insert("6. Количество учавствующих в боевых действиях:",query.value(rec.indexOf("counte_ls_bd")).toString());
        ls_info_date->insert(6,map);
        map.clear();
        map.insert("7. МПС по призыву:",query.value(rec.indexOf("mps_priz_ls")).toString());
        ls_info_date->insert(7,map);
        map.clear();
        map.insert("8. МПС по контракту:",query.value(rec.indexOf("mps_konrt_ls")).toString());
        ls_info_date->insert(8,map);
        map.clear();
        map.insert("9. МПС офицерского состава:",query.value(rec.indexOf("mps_of_ls")).toString());
        ls_info_date->insert(9,map);
        map.clear();
        map.insert("10. Краткое наименование формирования:",query.value(rec.indexOf("short_name_ls")).toString());
        ls_info_date->insert(10,map);
        map.clear();
        return *ls_info_date;
}
QMap<QString, QMap<QString, QString> > ReportData::ls_info_coord(int id_object)
{
           QString name_obj;
           ls_elem_obj = new QMap <QString,QMap<QString,QString> >;
           ls_elem_obj->clear();
           QSqlQuery query;

           QString str = "SELECT *\
                          FROM    coord_ls cls, coordinates coord\
                          WHERE  (cls.id_ls = ?)and \
                          (cls.id_coordinates=coord.id_coordinates)";
           query.prepare(str);
           query.addBindValue(id_object);
               if(!query.exec())
               {
                   QString sss = query.lastError().text();
                   return *ls_elem_obj;
               }
           QSqlRecord rec = query.record();

           QMap<QString, QString> map;
           query.next();
           name_obj="11. Координаты воинского формирования:";
                       map.clear();
                       QString w = "X= ";
                       w.append(query.value(rec.indexOf("x_coordinates")).toString());
                       w.append("<br>Y= ");
                       w.append(query.value(rec.indexOf("y_coordinates")).toString());
                       map.insert("Прямоугольные координаты",w);

                       QString q = "Широта: ";
                       q.append(query.value(rec.indexOf("latitude_wgs_84_g")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("latitude_wgs_84_m")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("latitude_wgs_84_s")).toString());
                       q.append("<br>Долгота: ");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_g")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_m")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_s")).toString());

                       map.insert("Геодезические координаты",q);
                       ls_elem_obj->insert(name_obj,map);

            return *ls_elem_obj;
  }
QMap<int, QMap<QString, QString> > ReportData::sc_info(int id_object)
{
    sc_info_date = new QMap<int, QMap<QString, QString> >;
    sc_info_date->clear();


    QSqlQuery query;
    query.prepare ("SELECT sc.name_special_conditions, sc.description_special_conditions, tsc.name_type_special_conditions, region.name_region \
                   FROM special_conditions sc, type_special_conditions tsc, region \
                   WHERE (id_special_conditions = ?) \
                   AND sc.id_type_special_conditions = tsc.id_type_special_conditions \
                   AND region.id_region = sc.id_region");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return *sc_info_date;
    }
    QString foto_name;
    foto_name = get_sc_foto_from_DB(id_object);

    QMap<QString, QString> map;
    QSqlRecord rec = query.record();
    query.next();
        map.clear();
        map.insert("Фотография:","<CENTER><IMG BORDER=\"0\" SRC=\""+foto_name+"\" height=200> </CENTER>");
        sc_info_date->insert(1,map);
        map.clear();
        map.insert("1. Наименование особого условия:",query.value(rec.indexOf("name_special_conditions")).toString());
        sc_info_date->insert(2,map);
        map.clear();
        map.insert("2. Наименование региона:",query.value(rec.indexOf("name_region")).toString());
        sc_info_date->insert(3,map);
        map.clear();
        map.insert("3. Тип особого условия:",query.value(rec.indexOf("name_type_special_conditions")).toString());
        sc_info_date->insert(4,map);
        map.clear();
        map.insert("4. Описание особого условия:",query.value(rec.indexOf("description_special_conditions")).toString());
        sc_info_date->insert(5,map);
        map.clear();
        return *sc_info_date;
}
QMap<QString, QMap<QString, QString> > ReportData::sc_info_coord(int id_object)
{
           QString name_obj;
           sc_elem_obj = new QMap <QString,QMap<QString,QString> >;
           sc_elem_obj->clear();
           QSqlQuery query;

           QString str = "SELECT *\
                          FROM    coord_spec_cond csc, coordinates coord\
                          WHERE  (csc.id_special_conditions = ?)and \
                          (csc.id_coordinates=coord.id_coordinates)";
           query.prepare(str);
           query.addBindValue(id_object);
               if(!query.exec())
               {
                   QString sss = query.lastError().text();
                   return *sc_elem_obj;
               }
           QSqlRecord rec = query.record();

           QMap<QString, QString> map;
           query.next();
           name_obj="5. Координаты особого условия:";
                       map.clear();
                       QString w = "X= ";
                       w.append(query.value(rec.indexOf("x_coordinates")).toString());
                       w.append("<br>Y= ");
                       w.append(query.value(rec.indexOf("y_coordinates")).toString());
                       map.insert("Прямоугольные координаты",w);

                       QString q = "Широта: ";
                       q.append(query.value(rec.indexOf("latitude_wgs_84_g")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("latitude_wgs_84_m")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("latitude_wgs_84_s")).toString());
                       q.append("<br>Долгота: ");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_g")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_m")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_s")).toString());

                       map.insert("Геодезические координаты",q);
                       sc_elem_obj->insert(name_obj,map);

            return *sc_elem_obj;
}
//***********************************************************************************************************
//=============================== ОТЧЕТЫ РЕГИОНЫ ============================================================
QMap<int, QMap<QString, QString> > ReportData::region_info(int id_object)
{
    region_info_date = new QMap<int, QMap<QString, QString> >;
    region_info_date->clear();


    QSqlQuery query;
    query.prepare ("SELECT reg.name_region, tr.name_type_region, reg.description_region \
                   FROM region reg, type_region tr \
                   WHERE (reg.id_region = ?) \
                   AND tr.id_type_region = reg.id_type_region");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return *region_info_date;
    }

    QMap<QString, QString> map;
    QSqlRecord rec = query.record();
    query.next();
        map.clear();
        map.insert("Наименование региона:",query.value(rec.indexOf("name_region")).toString());
        region_info_date->insert(1,map);
        map.clear();
        map.insert("Тип региона:",query.value(rec.indexOf("name_type_region")).toString());
        region_info_date->insert(2,map);
        map.clear();
        map.insert("Описание региона:",query.value(rec.indexOf("description_region")).toString());
        region_info_date->insert(3,map);
        map.clear();

        return *region_info_date;
}
QMap<int, QMap<QString, QString> > ReportData::region_info_pop(int id_object)
{
    reg_info_date = new QMap<int, QMap<QString, QString> >;
    reg_info_date->clear();

    QSqlQuery query;
    query.prepare ("SELECT reg.counte_population, reg.density_population, reg.emmigration_population, \
                   reg.immigration_population, reg.birth_population, reg.dead_population \
                   FROM region reg, type_region tr \
                   WHERE (reg.id_region = ?) \
                   AND tr.id_type_region = reg.id_type_region");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return *reg_info_date;
    }

    QMap<QString, QString> map;
    QSqlRecord rec = query.record();
    query.next();
        map.clear();
        map.insert("Население:",query.value(rec.indexOf("counte_population")).toString() + " чел.( " + query.value(rec.indexOf("density_population")).toString() + " чел на км<span style=' vertical-align:super;'>2</span>  плотность населения )");
        reg_info_date->insert(1,map);
        map.clear();

    QString str;
    QSqlQuery query_nations;
    QSqlRecord data;

    str=QString("SELECT name_nations,persent_nations \
                    FROM region,ls_nations,nations \
                    WHERE region.id_region = ls_nations.id_region \
                    AND ls_nations.id_nations = nations.id_nations AND region.id_region = %1").arg(id_object);

    query_nations.exec(str);
    data.clear();
    data = query_nations.record();

    number = query.value(rec.indexOf("counte_population")).toInt();
    QString q;
    while(query_nations.next())
    {
        name_nations_string = query_nations.value(data.indexOf("name_nations")).toString();
        persent_nations = query_nations.value(data.indexOf("persent_nations")).toDouble();
        number_nations = persent_nations/100*number;
        q.append("</P>  <P> - ");
        q.append(name_nations_string.toLocal8Bit());
        q.append(" (");
        q.append(QString("%1").arg(persent_nations).toLocal8Bit());
        q.append(" %, ");
        q.append(QString("%1").arg(number_nations).toLocal8Bit());
        q.append(" чел)");
    }
    map.insert("Национальный состав:",q);
    reg_info_date->insert(2,map);
    map.clear();
    map.insert("Уровень эммиграции:",query.value(rec.indexOf("emmigration_population")).toString());
    reg_info_date->insert(3,map);
    map.clear();
    map.insert("Уровень иммиграции:",query.value(rec.indexOf("immigration_population")).toString());
    reg_info_date->insert(4,map);
    map.clear();
    map.insert("Уровень рождаемости:",query.value(rec.indexOf("birth_population")).toString());
    reg_info_date->insert(5,map);
    map.clear();
    map.insert("Уровень смертности:",query.value(rec.indexOf("dead_population")).toString());
    reg_info_date->insert(6,map);
    map.clear();

    number_sex_m = 0;
    number_sex_w = 0;

    number_m = 0;
    number_w = 0;
    QStringList list;
    QString str_pol,str_percent;
    str_pol=QString("SELECT persent_sex_m FROM pop_sex WHERE id_region = %1").arg(id_object);
    QSqlQuery query_pol;
    query_pol.exec(str_pol);
    data.clear();
    data = query_pol.record();

    while(query_pol.next())
    {
        number = query_pol.value(data.indexOf("persent_sex_m")).toDouble();
        number_sex_m = number;
    }

    str_percent = QString("SELECT persent_sex_w FROM pop_sex WHERE id_region = %1").arg(id_object);
    query_pol.clear();
    query_pol.exec(str_percent);
    data.clear();
    data = query_pol.record();
    QString q_pol;
    while(query_pol.next())
    {
        number = query_pol.value(data.indexOf("persent_sex_w")).toDouble();
        number_sex_w = number;
    }
    if(number_sex_m==0)
    {
        map.insert("Половой состав:","Данных нет");
        reg_info_date->insert(7,map);
        map.clear();
    }
    else
    {
        number = query.value(rec.indexOf("counte_population")).toInt();
        number_m = number_sex_m/100*number;
        number_w = number - number_m;

        q_pol.append(" </P>  <P> Мужской пол: "); q_pol.append(QString("%1").arg(number_m).toLocal8Bit());
        q_pol.append(" чел. (");q_pol.append(QString("%1").arg(number_sex_m).toLocal8Bit()); q_pol.append("%)");
        q_pol.append(" </P> <P>  Женский пол: "); q_pol.append(QString("%1").arg(number_w).toLocal8Bit());
        q_pol.append(" чел. (");q_pol.append(QString("%1").arg(number_sex_w).toLocal8Bit()); q_pol.append("%)");

        map.insert("Половой состав:",q_pol);
        reg_info_date->insert(7,map);
        map.clear();
    }

    QString str_age;
    str_age=QString("SELECT name_age,persent_age FROM age,pop_age WHERE pop_age.id_age = age.id_age AND id_region = %1").arg(id_object);
    QSqlQuery query_age;
    QString q_age;
    query_age.exec(str_age);
    data.clear();
    data = query_age.record();
    if(query_age.size()==0)
    {
        map.insert("Возрастной состав:","Данных нет");
        reg_info_date->insert(8,map);
        map.clear();
    }
    else
    {
     while(query_age.next())
        {
            name_age_string = query_age.value(data.indexOf("name_age")).toString();
            persent_age_string = query_age.value(data.indexOf("persent_age")).toString();
            q_age.append("</P>  <P>");
            q_age.append(name_age_string.toLocal8Bit());
            q_age.append(" - ");
            q_age.append(persent_age_string.toLocal8Bit());
            q_age.append("%");
        }
        map.insert("Возрастной состав:",q_age);
        reg_info_date->insert(8,map);
        map.clear();
    }
    QString str_conf,q_conf;
    str_conf=QString("SELECT name_confessions,persent_confessions FROM ls_confessions,confessions WHERE id_region = %1 AND ls_confessions.id_confessions = confessions.id_confessions").arg(id_object);
    QSqlQuery query_conf;
    query_conf.exec(str_conf);
    data.clear();
    data = query_conf.record();
    if(query_conf.size()==0)
    {
        map.insert("Религиозный состав:","Данных нет");
        reg_info_date->insert(9,map);
        map.clear();
    }
    else
    {
    while(query_conf.next())
        {
            name_confessions = query_conf.value(data.indexOf("name_confessions")).toString();
            persent_confessions = query_conf.value(data.indexOf("persent_confessions")).toString();

            q_conf.append(" </P>  <P>");
            q_conf.append(name_confessions.toLocal8Bit());
            q_conf.append(" - ");
            q_conf.append(persent_confessions.toLocal8Bit());
            q_conf.append("%");
        }
    map.insert("Религиозный состав:",q_conf);
    reg_info_date->insert(9,map);
    map.clear();

    }
    return *reg_info_date;
}
QMap<int, QMap<QString, QString> > ReportData::region_info_smi(int id_object)
{
    region_info_date = new QMap<int, QMap<QString, QString> >;
    region_info_date->clear();
    QMap<QString, QString> map;
    QSqlQuery query,query1,query2;
    QSqlRecord data,data1,data2;
    QString str,str1,str2,q_pos;
    int l=0;
    int d=0;

    str=QString("SELECT id_position_smi,name_position_smi FROM position_smi");
    query.clear();
    query.exec(str);
    data.clear();
    data = query.record();
    QString name_position_smi,nametype_smi,name_smi;
    int id_position_smi,id_type_smi;

    while(query.next())
    {
        name_position_smi = query.value(data.indexOf("name_position_smi")).toString();
        id_position_smi = query.value(data.indexOf("id_position_smi")).toInt();

        str1 = QString("SELECT id_type_smi,nametype_smi FROM type_smi");
        query1.clear();
        query1.exec(str1);
        data1.clear();
        data1 = query1.record();
        l=0;

        while(query1.next())
        {
            nametype_smi = query1.value(data1.indexOf("nametype_smi")).toString();
            id_type_smi = query1.value(data1.indexOf("id_type_smi")).toInt();

            str2=QString("SELECT smi.name_smi \
                         FROM smi,smi_region \
                         WHERE smi_region.id_smi = smi.id_smi \
                         AND id_position_smi = %1 AND id_type_smi = %2 \
                         AND  id_region = %3").arg(id_position_smi).arg(id_type_smi).arg(id_object);
            query2.clear();
            query2.exec(str2);
            data2.clear();
            data2 = query2.record();
            smi_number = query2.size();
            if(smi_number>0)
            {
                l++;
                d=1;
                if(l<2)
                {
                    q_pos.append(" </P>  <P> </strong> <strong>");
                    q_pos.append(name_position_smi.toLocal8Bit() + ": ");
                }
                q_pos.append("</P> <P> ");
                q_pos.append(nametype_smi.toLocal8Bit());
                q_pos.append(": ");
                q_pos.append(QString("%1").arg(smi_number).toLocal8Bit());

                while(query2.next())
                {
                    name_smi = query2.value(data2.indexOf("name_smi")).toString();
                    q_pos.append("</P>  <P> - ");
                    q_pos.append(name_smi.toLocal8Bit());

                    map.insert("Позиция СМИ:",q_pos);
                    region_info_date->insert(1,map);
                    map.clear();
                }
            }
        }

    }

    if(d==0)
    map.insert("Информация о CМИ:","Данных нет");
    region_info_date->insert(2,map);
    map.clear();

    return *region_info_date;
}
QMap<int, QMap<QString, QString> > ReportData::region_info_group(int id_object){

    region_info_date = new QMap<int, QMap<QString, QString> >;
    region_info_date->clear();

    QMap<QString, QString> map;
    QSqlQuery query,query1,query2;
    QSqlRecord data,data1,data2;
    QString str,str1,str2,q_pos;
    QString name_trend_groups,name_sphere_groups,name_groups;
    int id_trend_groups,id_sphere_groups;
    int l=0;
    int d=0;
    str=QString("SELECT id_trend_groups,name_trend_groups FROM trend_groups");
    query.clear();
    query.exec(str);
    data.clear();
    data = query.record();

    while(query.next())
    {
        name_trend_groups = query.value(data.indexOf("name_trend_groups")).toString();
        id_trend_groups = query.value(data.indexOf("id_trend_groups")).toInt();

        str1 = QString("SELECT id_sphere_groups,name_sphere_groups FROM sphere_groups");
        query1.clear();
        query1.exec(str1);
        data1.clear();
        data1 = query1.record();
        l=0;

        while(query1.next())
        {
            name_sphere_groups = query1.value(data1.indexOf("name_sphere_groups")).toString();
            id_sphere_groups = query1.value(data1.indexOf("id_sphere_groups")).toInt();

            str2=QString("SELECT name_groups \
                         FROM groups \
                         WHERE id_sphere_groups = %1 AND id_trend = %2 \
                         AND  id_region = %3").arg(id_sphere_groups).arg(id_trend_groups).arg(id_object);
            query2.clear();
            query2.exec(str2);
            data2.clear();
            data2 = query2.record();
            smi_number = query2.size();
            if(smi_number>0)
            {
                l++;
                d=1;
                if(l<2)
                {
                    q_pos.append(" </P>  <P> </strong> <strong>");
                    q_pos.append(name_trend_groups.toLocal8Bit() + ": ");
                }
                q_pos.append("</P>  <P>");
                q_pos.append(name_sphere_groups.toLocal8Bit());
                q_pos.append(": ");
                q_pos.append(QString("%1").arg(smi_number).toLocal8Bit());

                while(query2.next())
                {
                    name_groups = query2.value(data2.indexOf("name_groups")).toString();
                    q_pos.append("</P>  <P> - ");
                    q_pos.append(name_groups.toLocal8Bit());

                    map.insert("Направленность организации:",q_pos);
                    region_info_date->insert(1,map);
                    map.clear();
                }
            }
        }
    }
    if(d==0)
    map.insert("Информация о организации:","Данных нет");
    region_info_date->insert(2,map);
    map.clear();

    return *region_info_date;
}
QMap<QString, QString> ReportData::region_info_factor(int id_object){

    info_date = new QMap<QString, QString>;
    info_date->clear();

    QSqlQuery query;
    QSqlRecord data;
    QString str,q_pos;

    str.clear();
    query.clear();
    data.clear();

    str = QString("SELECT * FROM region WHERE id_region = %1").arg(id_object);
    query.exec(str);
    data=query.record();

    for(int i=0;i<33;i++) factori[i]=0;
    factorflag=0;

    while(query.next())
    {
        int i=0;
        factori[i] = query.value(data.indexOf("poverty_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("price_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("education")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("trust_vs_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("support_vs_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("ability_vs_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("proposition_org_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("opposition_org_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("position_vip")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("unemployment_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("refugees")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("demography")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("availability_smi_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("smi_o_vs")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("protection_iti")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("ungov_org")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("patriotic_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("crim_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("corruption_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("shadow_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("illegal_migration_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("extremism_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("prison_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("protest_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("opg_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("drug_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("conflict_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("cooperation_ro")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("cult_object_population")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("autoritet_liders")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("regard_liders")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("religion_ls_enemy")).toDouble(); i++;
        factori[i] = query.value(data.indexOf("religion_ls_their")).toDouble();

    }
    factor_text << "Высокий уровень бедности и доли населения с денежными доходами, ниже региональной величины прожиточного минимума"
                << "Высокий уровень цен на продукты и услуги первой необходимости."
                << "Низкий уровень образования населения, недостаточное количество высших учебных заведений."
                << "Низкая степень доверия населения органам государственной власти, командованию Вооруженных Сил."
                << "Низкая степень поддержки населением действий Вооруженных Сил."
                << "Неспособность и отсутствие возможностей государственных (региональных) структур оказать содействие Вооружённым Силам в выполнении задач в период непосредственной угрозы агрессии и военное время."
                << "Низкая степень влияния на общественное мнение населения основных политических партий, неправительственных, общественных и религиозных организаций, выступающих в поддержку государства и Вооруженных сил."
                << "Наличие оппозиционных, радикальных политических движений и организаций (в том числе и молодежных) и достаточно высокий уровень их поддержки населением. "
                << "Негативная позиция государственных (региональных) авторитетных деятелей политики, культуры, искусства по отношению к Вооруженным Силам."
                << "Высокий уровень безработицы в регионе."
                << "Наличие беженцев из других регионов Российской Федерации, сопредельных с ней территорий."
                << "Сложная демографическая ситуация в регионе."
                << "Низкий уровень информатизации региона и степень доступности средств массовой информации и коммуникации, затрудняющие ведение пропаганды и контрпропаганды."
                << "Деструктивная направленность информации, публикуемой в региональных СМИ в отношении Вооруженных Сил."
                << "Низкая степень защищенности объектов телерадиовещания, сотовой связи, инфо - и телекоммуникационной инфраструктуры региона."
                << "Высокая степень активности неправительственных организаций и фондов деструктивной направленности."
                << "Низкий уровень сформированности патриотического сознания населения."
                << "Высокий уровень преступности в регионе."
                << "Высокая степень коррумпированности органов власти."
                << "Высокая степень влияния теневого сектора экономики и финансов региона на общественное мнение."
                << "Наличие нелегальных миграционных потоков."
                << "Наличие экстремистских проявлений и НВФ."
                << "Большое количество исправительно-трудовых учреждений, и число осужденных."
                << "Высокий уровень протестной активности населения."
                << "Наличие организованных преступных группировок."
                << "Наличие в peгионе путей незаконного экспорта (импорта) оружия и наркотиков."
                << "Наличие межнациональных, межэтнических конфликтов.";

    for(int i=0;i<33;i++) if(factori[i]>0 && factori[i]<=0.3) factorflag=1;
    if(factorflag == 0)
    {
        q_pos.append("</P>  <P> <B> <CENTER> Факторы, дестабилизирующие моральную обстановку в регионе, отсутствуют, либо о них неизвестно. </CENTER></B>  ");
        info_date->insert("",q_pos);
     }
    else
    {
        q_pos.append("</P>  <P> <B> <CENTER> Факторами, дестабилизирующими моральную обстановку в регионе, являются: </CENTER></B>  ");


        for(int i=0;i<26;i++)
        {
            if(factori[i]>0 && factori[i]<=0.3)
            {
                q_pos.append(" </P>  <P> - ");
                q_pos.append(factor_text.at(i).toLocal8Bit());
            }
            info_date->insert("",q_pos);

        }
    }


    return *info_date;
}
QMap<QString, QString> ReportData::region_info_itog(float rez_){

    info_date = new QMap<QString, QString>;


    QString q_pos;

    if(rez_ < 0.3)
        q_pos.append(" </P>  <P> <b> <CENTER> Обстановка затрудняет выполнение задач </CENTER> </b>");
    else
    {
        if(rez_ > 0.3 && rez_ <0.5)
        q_pos.append(" </P>  <P> <b> <CENTER> Обстановка не оказывает существенного влияния на выполнение задач </CENTER> </b>");
        else
        q_pos.append(" </P>  <P> <b> <CENTER> Обстановка способствует выполнению задач </CENTER> </b>");

    }

    info_date->insert("",q_pos);

    return *info_date;
}
//****************************************************************************************************
//****************************************************************************************************
//============================== отчеты по средствам =================================================

QMap<int, QMap<QString, QString> > ReportData::mpo_pso_smi_info(int id_object)
{
    mpsmi_info_date = new QMap<int, QMap<QString, QString> >;
    mpsmi_info_date->clear();


    QSqlQuery query;

    query.prepare ("SELECT mpo_pso.name_mpo_pso, mpo_pso.counte_mpo_pso, mpo_pso.description_mpo_pso, mpo_pso.image_mpo_pso, mpo_pso.id_ls, \
                   mpo_pso.id_smi, mpo_pso.id_groups, type_mpo_pso.name_type_mpo_pso, smi.name_smi \
                   FROM mpo_pso , type_mpo_pso, smi \
                   WHERE mpo_pso.id_mpo_pso = ? \
                   AND mpo_pso.id_type_mpo_pso = type_mpo_pso.id_type_mpo_pso \
                   AND mpo_pso.id_smi = smi.id_smi");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return *mpsmi_info_date;
    }

    QMap<QString, QString> map;
    QSqlRecord rec = query.record();
    QString foto_name;
    foto_name = get_mpo_pso_foto_from_DB(id_object);

    query.next();
        map.clear();
        map.insert("1. Наименование средства СМИ:",query.value(rec.indexOf("name_mpo_pso")).toString());
        mpsmi_info_date->insert(1,map);
        map.clear();
        map.insert("2. Фотография:","<CENTER><IMG BORDER=\"0\" SRC=\""+foto_name+"\" height=200> </CENTER>");
        mpsmi_info_date->insert(2,map);
        map.clear();
        map.insert("3. Подчиненность:",query.value(rec.indexOf("name_smi")).toString());
        mpsmi_info_date->insert(3,map);
        map.clear();
        map.insert("4. Количество средств:",query.value(rec.indexOf("counte_mpo_pso")).toString());
        mpsmi_info_date->insert(4,map);
        map.clear();
        map.insert("5. Тип средства СМИ:",query.value(rec.indexOf("name_type_mpo_pso")).toString());
        mpsmi_info_date->insert(5,map);
        map.clear();
        map.insert("6. Описание средства СМИ:",query.value(rec.indexOf("description_mpo_pso")).toString());
        mpsmi_info_date->insert(6,map);
        map.clear();

        return *mpsmi_info_date;
}
QMap<int, QMap<QString, QString> > ReportData::mpo_pso_ls_info(int id_object)
{
    mpls_info_date = new QMap<int, QMap<QString, QString> >;
    mpls_info_date->clear();

    QSqlQuery query;

    query.prepare ("SELECT mpo_pso.name_mpo_pso, mpo_pso.counte_mpo_pso, mpo_pso.description_mpo_pso, mpo_pso.image_mpo_pso, mpo_pso.id_ls, \
                   mpo_pso.id_smi, mpo_pso.id_groups, type_mpo_pso.name_type_mpo_pso, ls.name_ls \
                   FROM mpo_pso , type_mpo_pso, ls \
                   WHERE mpo_pso.id_mpo_pso = ? \
                   AND mpo_pso.id_type_mpo_pso = type_mpo_pso.id_type_mpo_pso \
                   AND mpo_pso.id_ls = ls.id_ls");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return *mpls_info_date;
    }

    QMap<QString, QString> map;
    QSqlRecord rec = query.record();
    QString foto_name;
    foto_name = get_mpo_pso_foto_from_DB(id_object);

    query.next();
        map.clear();
        map.insert("1. Наименование средства Воинского формирования:",query.value(rec.indexOf("name_mpo_pso")).toString());
        mpls_info_date->insert(1,map);
        map.clear();
        map.insert("2. Фотография:","<CENTER><IMG BORDER=\"0\" SRC=\""+foto_name+"\" height=200> </CENTER>");
        mpls_info_date->insert(2,map);
        map.clear();
        map.insert("3. Подчиненность:",query.value(rec.indexOf("name_ls")).toString());
        mpls_info_date->insert(3,map);
        map.clear();
        map.insert("4. Количество средств:",query.value(rec.indexOf("counte_mpo_pso")).toString());
        mpls_info_date->insert(4,map);
        map.clear();
        map.insert("5. Тип средства Воинского формирования:",query.value(rec.indexOf("name_type_mpo_pso")).toString());
        mpls_info_date->insert(5,map);
        map.clear();
        map.insert("6. Описание средства Воинского формирования:",query.value(rec.indexOf("description_mpo_pso")).toString());
        mpls_info_date->insert(6,map);
        map.clear();

        return *mpls_info_date;
}
QMap<int, QMap<QString, QString> > ReportData::mpo_pso_gr_info(int id_object)
{
    mpgr_info_date = new QMap<int, QMap<QString, QString> >;
    mpgr_info_date->clear();

    QSqlQuery query;

    query.prepare ("SELECT mpo_pso.name_mpo_pso, mpo_pso.counte_mpo_pso, mpo_pso.description_mpo_pso, mpo_pso.image_mpo_pso, mpo_pso.id_ls, \
                   mpo_pso.id_smi, mpo_pso.id_groups, type_mpo_pso.name_type_mpo_pso, groups.name_groups \
                   FROM mpo_pso , type_mpo_pso, groups \
                   WHERE mpo_pso.id_mpo_pso = ? \
                   AND mpo_pso.id_type_mpo_pso = type_mpo_pso.id_type_mpo_pso \
                   AND mpo_pso.id_groups = groups.id_groups");

    query.addBindValue(id_object);
    if(!query.exec())
    {
        QString sss = query.lastError().text();
        return *mpgr_info_date;
    }

    QMap<QString, QString> map;
    QSqlRecord rec = query.record();
    QString foto_name;
    foto_name = get_mpo_pso_foto_from_DB(id_object);

    query.next();
        map.clear();
        map.insert("1. Наименование средства организации:",query.value(rec.indexOf("name_mpo_pso")).toString());
        mpgr_info_date->insert(1,map);
        map.clear();
        map.insert("2. Фотография:","<CENTER><IMG BORDER=\"0\" SRC=\""+foto_name+"\" height=200> </CENTER>");
        mpgr_info_date->insert(2,map);
        map.clear();
        map.insert("3. Подчиненность:",query.value(rec.indexOf("name_groups")).toString());
        mpgr_info_date->insert(3,map);
        map.clear();
        map.insert("4. Количество средств:",query.value(rec.indexOf("counte_mpo_pso")).toString());
        mpgr_info_date->insert(4,map);
        map.clear();
        map.insert("5. Тип средства организации:",query.value(rec.indexOf("name_type_mpo_pso")).toString());
        mpgr_info_date->insert(5,map);
        map.clear();
        map.insert("6. Описание средства организации:",query.value(rec.indexOf("description_mpo_pso")).toString());
        mpgr_info_date->insert(6,map);
        map.clear();

        return *mpgr_info_date;
}
QMap<QString, QMap<QString, QString> > ReportData::mpo_pso_info_coord(int id_object)
{
           QString name_obj;
           mpo_elem_obj = new QMap <QString,QMap<QString,QString> >;
           mpo_elem_obj->clear();
           QSqlQuery query;

           QString str = "SELECT *\
                          FROM    coord_mpo_pso mpo, coordinates coord\
                          WHERE  (mpo.id_mpo_pso = ?)and \
                          (mpo.id_coordinates=coord.id_coordinates)";
           query.prepare(str);
           query.addBindValue(id_object);
               if(!query.exec())
               {
                   QString sss = query.lastError().text();
                   return *mpo_elem_obj;
               }
           QSqlRecord rec = query.record();

           QMap<QString, QString> map;
           query.next();
           name_obj="7. Координаты средства:";
                       map.clear();
                       QString w = "X= ";
                       w.append(query.value(rec.indexOf("x_coordinates")).toString());
                       w.append("<br>Y= ");
                       w.append(query.value(rec.indexOf("y_coordinates")).toString());
                       map.insert("Прямоугольные координаты",w);

                       QString q = "Широта: ";
                       q.append(query.value(rec.indexOf("latitude_wgs_84_g")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("latitude_wgs_84_m")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("latitude_wgs_84_s")).toString());
                       q.append("<br>Долгота: ");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_g")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_m")).toString());
                       q.append("-");
                       q.append(query.value(rec.indexOf("longitude_wgs_84_s")).toString());

                       map.insert("Геодезические координаты",q);
                       mpo_elem_obj->insert(name_obj,map);

            return *mpo_elem_obj;
  }
//=========================== the end работа с report по срелствам =============================================
//**************************************************************************************************************

//=========================== методы работают с картинками из БД ===============================================
QString ReportData::get_pers_foto_from_DB(int id_persones)
{
        QDir dir;
        QString currentPath = dir.tempPath();
        QPixmap pixmap;
        QString pathStr = currentPath + "/pixmap_pers.png";
        QFile file(pathStr);

        if(file.exists()){
           file.remove(pathStr);
        }
        QSqlQuery query;
 
        QString str = QString("SELECT pers.image_persones FROM  persones pers, type_persones WHERE pers.id_persones = %1 AND pers.id_type_persones = type_persones.id_type_persones").arg(id_persones);

        if(!query.exec(str))
        {
            QString sss = query.lastError().text();
            return pathStr;
        }
            QSqlRecord rec = query.record();
            while(query.next()){
            pixmap.loadFromData(query.value(rec.indexOf("image_persones")).toByteArray());
            pixmap.save(pathStr, "PNG");
        }

    return pathStr;
}
QString ReportData::get_mpo_pso_foto_from_DB(int id_mpo)
{
        QDir dir;
        QString currentPath = dir.tempPath();
        QPixmap pixmap;
        QString pathStr = currentPath + "/pixmap_pers.png";
        QFile file(pathStr);

        if(file.exists()){
           file.remove(pathStr);
        }
        QSqlQuery query;

        QString str = QString("SELECT mpo_pso.image_mpo_pso FROM  mpo_pso WHERE mpo_pso.id_mpo_pso = %1").arg(id_mpo);

        if(!query.exec(str))
        {
            QString sss = query.lastError().text();
            return pathStr;
        }
            QSqlRecord rec = query.record();
            while(query.next()){
            pixmap.loadFromData(query.value(rec.indexOf("image_mpo_pso")).toByteArray());
            pixmap.save(pathStr, "PNG");
        }

    return pathStr;
}
QString ReportData::get_sc_foto_from_DB(int id_sc)
{
        QDir dir;
        QString currentPath = dir.tempPath();
        QPixmap pixmap;
        QString pathStr = currentPath + "/pixmap_pers.png";
        QFile file(pathStr);

        if(file.exists()){
           file.remove(pathStr);
        }
        QSqlQuery query;

        QString str = QString("SELECT image_special_conditions FROM  special_conditions WHERE id_special_conditions = %1").arg(id_sc);

        if(!query.exec(str))
        {
            QString sss = query.lastError().text();
            return pathStr;
        }
            QSqlRecord rec = query.record();
            while(query.next()){
            pixmap.loadFromData(query.value(rec.indexOf("image_special_conditions")).toByteArray());
            pixmap.save(pathStr, "PNG");
        }

    return pathStr;
}
