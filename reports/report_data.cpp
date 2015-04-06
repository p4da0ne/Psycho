#include "report_data.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>


ReportData::ReportData(QObject *parent)
    : QObject(parent)
{
  
    
}

ReportData::~ReportData()
{
    
}

//========== Получение данных об объекте поражения по его id ===========

QMap<QString,QString> ReportData::get_obj_info(int id_object)
{
    obj_info = new QMap<QString, QString>;
	obj_info->clear();			
	QSqlQuery query;
	query.prepare("SELECT * \
		           FROM   object_class c, object_type_in_class tc, object_type t,object_importance i, nationality n,object_status s, object_pattern p \
		           WHERE  (id_object_pattern = ?) and (s.id_object_status=p.id_object_status)and \
				          (n.id_nationality=p.id_nationality) and (i.id_object_importance=p.id_object_importance) and\
						  (t.id_object_type=p.id_object_type) and (tc.id_type=p.id_type) and (tc.id_object_class=c.id_object_class)");
	query.addBindValue(id_object);
	if(!query.exec())
	{
		QString sss = query.lastError().text();
		return *obj_info;
	}
	QSqlRecord rec = query.record();
	query.next();
		obj_info->insert("id_object",query.value(rec.indexOf("id_object_pattern")).toString());
		obj_info->insert("object_name",query.value(rec.indexOf("object_name")).toString());
		
		obj_info->insert("object_class",query.value(rec.indexOf("class_name")).toString());           //object_class
		obj_info->insert("unique_number",query.value(rec.indexOf("unique_number")).toString());
		obj_info->insert("nationality",query.value(rec.indexOf("country_name")).toString());          //nationality
		obj_info->insert("object_status",query.value(rec.indexOf("status_name")).toString());         //object_status
		obj_info->insert("object_type",query.value(rec.indexOf("type_name")).toString());             // --- тип ОПП (стационарный, подвижный,...) -- таблица в БД - object_type
		
		obj_info->insert("size_x",query.value(rec.indexOf("size_x")).toString());
		obj_info->insert("size_y",query.value(rec.indexOf("size_y")).toString());
	    obj_info->insert("destroy_recomendation",query.value(rec.indexOf("destroy_recomendation")).toString());
	    obj_info->insert("description",query.value(rec.indexOf("description")).toString());
	//  obj_info->insert("type",query.value(rec.indexOf("short_name")).toString());                    // --- тип ОПП в классе объектов (система энерго..., система промышл...) -- таблица в БД - object_type_in_class
        obj_info->insert("object_importance",query.value(rec.indexOf("importance_name")).toString());  //object_importance
	return *obj_info;
}




// ++++++++++++ информация по уязвимым элементам ++++++++++++++
QMultiMap<QString,QString> ReportData::get_obj_v(int id_object)
{
    obj_info_v = new QMultiMap<QString, QString>;
	 obj_info_v->clear();			
	QSqlQuery query;	
	query.prepare("SELECT * \
		           FROM   object_coordinates c, vulnerability_element v, object_pattern p \
		           WHERE  (id_object_pattern = ?) and (c.id_object_pattern=p.id_object_pattern) and \
				          (v.id_object_pattern=p.id_object_pattern)");


	query.addBindValue(id_object);
	if(!query.exec())
	{
		QString sss = query.lastError().text();
		return *obj_info_v;
	}
	QSqlRecord rec = query.record();
	query.next();
	//неправильно
	obj_info_v->insert("w",query.value(rec.indexOf("element_name")).toString());


return *obj_info_v;
}