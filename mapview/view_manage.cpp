#include "view_manage.h"
#include <QSqlQuery>
#include <QsqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QMap>

ViewManage::ViewManage(QObject *parent)
    : QObject(parent)
{
  
    
}

ViewManage::~ViewManage()
{
    
}


//========== Получение данных об объекте поражения по его id ===========
QMap<QString,QString> ViewManage::get_obj_info(int id_object)
{
    obj_info = new QMap<QString, QString>;
	obj_info->clear();
	
		//от классифик-ции элем до уязвимых эл-тов
	QSqlQuery query;
	//QString str = QString("SELECT * FROM object_pattern WHERE id_object_pattern = %1").arg(id_object);
/*QString str = "SELECT object_class.class_name, object_type_in_class.type_name, \
			   object_pattern.unique_number, nationality.country_name, object_pattern.object_name, \
			   additional_property.property_name, additional_property.property_value, object_pattern.size_x, \
			   object_pattern.size_y, object_pattern.destroy_recomendation \
			   FROM (object_class INNER JOIN object_type_in_class ON object_class.id_object_class = \
			   object_type_in_class.id_object_class) INNER JOIN ((nationality INNER JOIN object_pattern ON \
			   nationality.id_nationality = object_pattern.id_nationality) INNER JOIN additional_property ON \
			   object_pattern.id_object_pattern = additional_property.id_object_pattern) ON object_type_in_class.id_type = \
			   object_pattern.id_type\
			   WHERE (((object_pattern.id_object_pattern)= ?))";*/
QString str = "SELECT object_class.class_name, object_type_in_class.short_name, \
			 object_pattern.unique_number, nationality.country_name, object_pattern.object_name, object_pattern.size_x, \
			 object_pattern.size_y, object_pattern.destroy_recomendation \
			 FROM object_class, object_type_in_class, nationality, object_pattern \
			 WHERE object_type_in_class.id_type = object_pattern.id_type \
			 AND nationality.id_nationality = object_pattern.id_nationality \
			 AND object_class.id_object_class =  object_type_in_class.id_object_class \
			 AND object_pattern.id_object_pattern = ?";
	query.prepare(str);

	query.addBindValue(id_object); 

	if(!query.exec())
	{
		QString sss = query.lastError().text();
		return *obj_info;
	}

	QSqlRecord rec = query.record();
	query.next();

	obj_info->insert("id_object",QString::number(id_object));
	obj_info->insert("class_name",query.value(rec.indexOf("class_name")).toString());
	obj_info->insert("type_name",query.value(rec.indexOf("short_name")).toString());
	obj_info->insert("unique_number",query.value(rec.indexOf("unique_number")).toString());
	obj_info->insert("country_name",query.value(rec.indexOf("country_name")).toString());
	obj_info->insert("object_name",query.value(rec.indexOf("object_name")).toString());
	obj_info->insert("description",query.value(rec.indexOf("description")).toString());
	//-------- foreign keys -------
	//obj_info->insert("property_value",query.value(rec.indexOf("property_value")).toString());
	obj_info->insert("size_x",query.value(rec.indexOf("size_x")).toString());
	obj_info->insert("size_y",query.value(rec.indexOf("size_y")).toString());
	obj_info->insert("destroy_recomendation",query.value(rec.indexOf("destroy_recomendation")).toString());    
	
	//координаты местоположения
//str = "SELECT object_coordinates.latitude_wgs_84_g FROM object_coordinates WHERE (((object_coordinates.id_object_pattern)=?))";

	str = "SELECT DISTINCT ON (object_coordinates.id_object_pattern) \
		  object_coordinates.latitude_wgs_84_g, object_coordinates.latitude_wgs_84_m, \
		  object_coordinates.latitude_wgs_84_s, object_coordinates.longitude_wgs_84_g, \
		  object_coordinates.longitude_wgs_84_m, object_coordinates.longitude_wgs_84_s, \
		  object_coordinates.latitude_sk_42_g, object_coordinates.latitude_sk_42_m, \
		  object_coordinates.latitude_sk_42_s, object_coordinates.longitude_sk_42_g, \
		  object_coordinates.longitude_sk_42_m, object_coordinates.longitude_sk_42_s, \
		  object_coordinates.x, object_coordinates.y, object_coordinates.h, object_coordinates.h_wgs_84, \
		  object_coordinates.h_sk_42 FROM object_coordinates WHERE (((object_coordinates.id_object_pattern)=?))";
 
	query.prepare(str);

	query.addBindValue(id_object);

	if(!query.exec())
	{
		QString sss = query.lastError().text();
		return *obj_info;
	}

	rec = query.record();
	query.next();
	//-----wgs_84
	QString coordinates = query.value(rec.indexOf("latitude_wgs_84_g")).toString()+QChar(176)+\
		query.value(rec.indexOf("latitude_wgs_84_m")).toString()+"'"+\
		query.value(rec.indexOf("latitude_wgs_84_s")).toString()+"\"";
	obj_info->insert("latitude_wgs_84",coordinates);
	coordinates = query.value(rec.indexOf("longitude_wgs_84_g")).toString()+QChar(176)+\
		query.value(rec.indexOf("longitude_wgs_84_m")).toString()+"'"+\
		query.value(rec.indexOf("longitude_wgs_84_s")).toString()+"\"";
	obj_info->insert("longitude_wgs_84",coordinates);
	//------sk_42
	coordinates = query.value(rec.indexOf("latitude_sk_42_g")).toString()+QChar(176)+\
		query.value(rec.indexOf("latitude_sk_42_m")).toString()+"'"+\
		query.value(rec.indexOf("latitude_sk_42_s")).toString()+"\"";
	obj_info->insert("latitude_sk_42",coordinates);
	coordinates = query.value(rec.indexOf("longitude_sk_42_g")).toString()+QChar(176)+\
		query.value(rec.indexOf("longitude_sk_42_m")).toString()+"'"+\
		query.value(rec.indexOf("longitude_sk_42_s")).toString()+"\"";
	obj_info->insert("longitude_sk_42",coordinates);	
	return *obj_info;


}

//======= Выборка данных обо всех объектах поражения, попадающих в квадрат (x1,y1;x2,y2)  =========
QMap<QString,QMap<QString,QString> > ViewManage::get_all_objects_info(double x1,double y1, double x2,double y2 )
{
 all_obj = new QMap<QString,QMap<QString,QString> >;
 all_obj->clear();

 QSqlQuery query;
 QString str = QString("SELECT DISTINCT ON (c.id_object_pattern) o.id_object_pattern, c.x, c.y, o.id_sign \
					   FROM object_coordinates c, object_pattern o, object_type_in_class t \
					   WHERE (x > %1) AND (y > %2) AND (x < %3) AND (y < %4) \
					   AND (c.id_object_pattern=o.id_object_pattern)").arg(x1).arg(y1).arg(x2).arg(y2);




	if(!query.exec(str))
	{
		QString sss = query.lastError().text();
		return *all_obj;
	}

	QSqlRecord rec = query.record();

	while(query.next())
	{
		QMap<QString,QString> data;
		data.insert("x",query.value(rec.indexOf("x")).toString());
		data.insert("y",query.value(rec.indexOf("y")).toString());

		
	
		if(query.value(rec.indexOf("id_sign")).isNull())
		{
		   data.insert("key","T693");
		}
		else
		{
			QSqlQuery query_sign;
			str = QString("SELECT sign_key FROM object_signs WHERE id_sign = %1").arg(query.value(rec.indexOf("id_sign")).toInt());
			if(!query_sign.exec(str))
			{
				data.insert("key","T693");
			}

			QSqlRecord rec_sign = query_sign.record();
			query_sign.next();
			data.insert("key",query_sign.value(rec_sign.indexOf("sign_key")).toString());
		}

		all_obj->insert(query.value(rec.indexOf("id_object_pattern")).toString(),data);
	}

	return *all_obj;
}
