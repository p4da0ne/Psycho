#include "view_manage.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <qmath.h>
#include <QSettings>
#include <my_mapaccess.h>
#include "mapview.h"

ViewManage::ViewManage(QObject *parent)
    : QObject(parent)
{
	//MainCodec = QTextCodec::codecForName("CP1251");
  	
	mapwin = new MapScroll();
	

	
	
}

ViewManage::~ViewManage()
{
    
}


//======================================================================================
//========= Метод перевода геодезических координат (WGS84) в прямоугольные =============
//======================================================================================
Coord* ViewManage::WGStoPlane(long int hMap,Coord *coordObject)
{
   Coord *tempCoord = new Coord;
   
   if(hMap == 0) return 0;

	MyMapAccess *map = new MyMapAccess;

   if(map->mapIsGeoSupported(hMap))
    {
        GEODEGREE N, E;
        double N_rad, E_rad, H;

		N.Degree = coordObject->getLatDegrees();
		N.Minute = coordObject->getLatMinutes();
		N.Second = coordObject->getLatSeconds();

        map->mapDegreeToRadian(&N, &N_rad);

		E.Degree = coordObject->getLongDegrees();
		E.Minute = coordObject->getLongMinutes();
		E.Second = coordObject->getLongSeconds();

        map->mapDegreeToRadian(&E, &E_rad);

        map->mapGeoWGS84ToPlane3D(hMap,&N_rad,&E_rad,&H);

        tempCoord->setX(N_rad);
		tempCoord->setY(E_rad);
		tempCoord->setH(H);
    }
	return tempCoord;
}

//================================================================================
//==== Метод возвращает список объектов SignData с информацией ===================
//==== для нанесения на карту и инициализации условных знаков средств СМИ ========
//================================================================================
QList<SignData*> ViewManage::getSmiMeans(long int hMap,double x1,double y1,double x2,double y2)
{
	QList<SignData*> smiMeansList;

	QSqlQuery query;
	QString str=QString("SELECT DISTINCT name_type_mpo_pso, coordinates.latitude_wgs_84_g,coordinates.latitude_wgs_84_m,coordinates.latitude_wgs_84_s,coordinates.longitude_wgs_84_g, \
						coordinates.longitude_wgs_84_m,coordinates.longitude_wgs_84_s, \
						type_mpo_pso.excode_type_mpo_pso, mpo_pso.id_mpo_pso, mpo_pso.semantika_digit1, mpo_pso.semantika_digit2, semantika_1 \
						FROM mpo_pso, coord_mpo_pso cmp, coordinates, type_mpo_pso \
						WHERE cmp.id_coordinates=coordinates.id_coordinates \
						AND mpo_pso.id_type_mpo_pso=type_mpo_pso.id_type_mpo_pso \
						AND type_mpo_pso.excode_type_mpo_pso <> '' \
						AND mpo_pso.id_smi > 0 ");
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{		
			int wgs_g = query.value(rec.indexOf("latitude_wgs_84_g")).toInt();
			int wgs_m = query.value(rec.indexOf("latitude_wgs_84_m")).toInt();
			double wgs_s = query.value(rec.indexOf("latitude_wgs_84_s")).toDouble();
			int long_wgs_g = query.value(rec.indexOf("longitude_wgs_84_g")).toInt();
			int long_wgs_m = query.value(rec.indexOf("longitude_wgs_84_m")).toInt();
			double long_wgs_s = query.value(rec.indexOf("longitude_wgs_84_s")).toDouble();
					
			///получить из запроса 6 параметров координат WGS
			Coord c1(wgs_g,wgs_m,wgs_s,long_wgs_g,long_wgs_m,long_wgs_s);
			
			Coord *c2 = WGStoPlane(hMap,&c1);		
			
			double x_coord = c2->getX();
			double y_coord = c2->getY();
			
			if(((x_coord > x1) && (y_coord > y1)) && ((x_coord < x2) && (y_coord < y2))) continue;	
			
			QString name_type_mpo_pso = query.value(rec.indexOf("name_type_mpo_pso")).toString();
			QString signCode = query.value(rec.indexOf("excode_type_mpo_pso")).toString();
			QString id_mpo_pso = query.value(rec.indexOf("id_mpo_pso")).toString();

			// дальность (радиус) действия, километры
			QString semantika_digit1_mpo_pso = query.value(rec.indexOf("semantika_digit1")).toString();
			// угол (направление) относительно горизонта против часовой стрелки, градусы
			QString semantika_digit2_mpo_pso = query.value(rec.indexOf("semantika_digit2")).toString();
			QString semantika_1_mpo_pso = query.value(rec.indexOf("semantika_1")).toString();

			QList<Coord*> coordList;
			Coord *coord = new Coord(x_coord,y_coord);	
			coordList.append(coord);
			
			///////////////////////////////////
			double radius;
			double angle;
			
			if(semantika_digit1_mpo_pso > 0)
			{
				radius = semantika_digit1_mpo_pso.toDouble()*250;
			}
			if(semantika_digit2_mpo_pso > 0)
			{
				angle = 5*3.14/2 - (semantika_digit2_mpo_pso.toDouble()*3.14/180);
			}

		 
			// если зачек радио-теле центра (свой или вражеский), то добавляем вторую метрику
			if ((signCode=="V0000169007")||(signCode=="V0000169029"))
			{	
				coord = new Coord(x_coord,y_coord+radius);
				coordList.append(coord);
			}


			if (signCode=="L00000060504") // самолет
			{	
				coord = new Coord(x_coord+120000,y_coord+140000);
				coordList.append(coord);
				coord = new Coord(x_coord+120000+70000,y_coord+140000-80000);
				coordList.append(coord);
			}


			// если передвижная звуковещательная станция, то добавляем вторую метрику 
			// (получаем ее как угол места и длину радиус-вектора, направленного из первой точки метрики)
			if (signCode=="V0000060505")
			{	
				double xx=4*radius*qCos(angle);
				double yy=4*radius*qSin(angle);
				
				coord = new Coord(x_coord+xx,y_coord+yy);
				coordList.append(coord);
			}
			
			
			QMap<long int,QString> semantic_map;

			semantic_map[17501] = id_mpo_pso;
			semantic_map[17502] = QString::number(SMI_MEANS);
			semantic_map[18]=semantika_digit1_mpo_pso;	// иногда это наполнение значка (в тех случаях, когда не "дальность")
			semantic_map[19]=semantika_1_mpo_pso;	// подпись значка
			semantic_map[32811]=semantika_digit1_mpo_pso;	//дальность действия средства
			semantic_map[32852]=semantika_digit2_mpo_pso;	//направление (угол) действия средства

			SignData *signData = new SignData(signCode,coordList,semantic_map);
				
			smiMeansList.append(signData);
		}
	}
	return smiMeansList;
}


//===================================================================================
//==== Метод возвращает список объектов SignData с информацией ======================
//==== для нанесения на карту и инициализации условных знаков средств формирований ==
//===================================================================================
QList<SignData*> ViewManage::getFormationsMeans()
{
	QList<SignData*> formationsMeansList;

	QSqlQuery query;
	QString str=QString("SELECT name_type_mpo_pso, coordinates.x_coordinates, coordinates.y_coordinates, \
						type_mpo_pso.excode_type_mpo_pso, id_mpo_pso, mpo_pso.semantika_digit1, mpo_pso.semantika_digit2, semantika_1 \
						FROM mpo_pso, coordinates, type_mpo_pso \
						WHERE mpo_pso.id_coordinates=coordinates.id_coordinates \
						AND mpo_pso.id_type_mpo_pso=type_mpo_pso.id_type_mpo_pso \
						AND type_mpo_pso.excode_type_mpo_pso <> '' \
						AND coordinates.x_coordinates<>0 \
						AND coordinates.y_coordinates<>0 \
						AND mpo_pso.id_ls > 0");
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{		
			QString name_type_mpo_pso = query.value(rec.indexOf("name_type_mpo_pso")).toString();
			long int x_coord=query.value(rec.indexOf("x_coordinates")).toInt();
			long int y_coord=query.value(rec.indexOf("y_coordinates")).toInt();
			QString signCode = query.value(rec.indexOf("excode_type_mpo_pso")).toString();
			QString id_mpo_pso = query.value(rec.indexOf("id_mpo_pso")).toString();

			// дальность (радиус) действия, километры
			QString semantika_digit1_mpo_pso = query.value(rec.indexOf("semantika_digit1")).toString();
			// угол (направление) относительно горизонта против часовой стрелки, градусы
			QString semantika_digit2_mpo_pso = query.value(rec.indexOf("semantika_digit2")).toString();
			QString semantika_1_mpo_pso = query.value(rec.indexOf("semantika_1")).toString();

			QList<Coord*> coordList;
			Coord *coord = new Coord(x_coord,y_coord);	
			coordList.append(coord);

			QMap<long int,QString> semantic_map;

			semantic_map[17501] = id_mpo_pso;
			semantic_map[17502] = QString::number(FORMATIONS_MEANS);
			semantic_map[18]=semantika_digit1_mpo_pso;	// иногда это наполнение значка (в тех случаях, когда не "дальность")
			semantic_map[19]=semantika_1_mpo_pso;	// подпись значка
			semantic_map[32811]=semantika_digit1_mpo_pso;	//дальность действия средства
			semantic_map[32852]=semantika_digit2_mpo_pso;	//направление (угол) действия средства

			SignData *signData = new SignData(signCode,coordList,semantic_map);
				
			formationsMeansList.append(signData);
		}
	}
	return formationsMeansList;
}

//===================================================================================
//==== Метод возвращает список объектов SignData с информацией ======================
//==== для нанесения на карту и инициализации условных знаков средств организаций ===
//===================================================================================
QList<SignData*> ViewManage::getGroupsMeans()
{
	QList<SignData*> groupsMeansList;

	QSqlQuery query;
	QString str=QString("SELECT name_type_mpo_pso, coordinates.x_coordinates, coordinates.y_coordinates, \
						type_mpo_pso.excode_type_mpo_pso, id_mpo_pso, mpo_pso.semantika_digit1, mpo_pso.semantika_digit2, semantika_1 \
						FROM mpo_pso, coordinates, type_mpo_pso \
						WHERE mpo_pso.id_coordinates=coordinates.id_coordinates \
						AND mpo_pso.id_type_mpo_pso=type_mpo_pso.id_type_mpo_pso \
						AND type_mpo_pso.excode_type_mpo_pso <> '' \
						AND coordinates.x_coordinates<>0 \
						AND coordinates.y_coordinates<>0 \
						AND mpo_pso.id_groups > 0");
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{		
			QString name_type_mpo_pso = query.value(rec.indexOf("name_type_mpo_pso")).toString();
			long int x_coord=query.value(rec.indexOf("x_coordinates")).toInt();
			long int y_coord=query.value(rec.indexOf("y_coordinates")).toInt();
			QString signCode = query.value(rec.indexOf("excode_type_mpo_pso")).toString();
			QString id_mpo_pso = query.value(rec.indexOf("id_mpo_pso")).toString();

			// дальность (радиус) действия, километры
			QString semantika_digit1_mpo_pso = query.value(rec.indexOf("semantika_digit1")).toString();
			// угол (направление) относительно горизонта против часовой стрелки, градусы
			QString semantika_digit2_mpo_pso = query.value(rec.indexOf("semantika_digit2")).toString();
			QString semantika_1_mpo_pso = query.value(rec.indexOf("semantika_1")).toString();

			QList<Coord*> coordList;
			Coord *coord = new Coord(x_coord,y_coord);	
			coordList.append(coord);

			QMap<long int,QString> semantic_map;

			semantic_map[17501] = id_mpo_pso;
			semantic_map[17502] = QString::number(GROUPS_MEANS);
			semantic_map[18]=semantika_digit1_mpo_pso;	// иногда это наполнение значка (в тех случаях, когда не "дальность")
			semantic_map[19]=semantika_1_mpo_pso;	// подпись значка
			semantic_map[32811]=semantika_digit1_mpo_pso;	//дальность действия средства
			semantic_map[32852]=semantika_digit2_mpo_pso;	//направление (угол) действия средства

			SignData *signData = new SignData(signCode,coordList,semantic_map);
				
			groupsMeansList.append(signData);
		}
	}
	return groupsMeansList;
}


//================================================================================
//==== Метод возвращает список объектов SignData с информацией ===================
//==== для нанесения на карту и инициализации условных знаков формирований =======
//================================================================================
QList<SignData*> ViewManage::getFormations()
{
	QList<SignData*> formationsList;

	QSqlQuery query;
	QString str=QString("SELECT name_ls, coordinates.x_coordinates, coordinates.y_coordinates, type_ls.excode_type_ls, ls.short_name_ls, id_ls \
						FROM ls, coordinates, type_ls WHERE ls.id_coordinates=coordinates.id_coordinates AND ls.id_type_ls=type_ls.id_type_ls\
						AND type_ls.excode_type_ls <> '' AND coordinates.x_coordinates<>0 AND coordinates.y_coordinates<>0 AND ls.short_name_ls <> ''");
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{
			QString name_ls = query.value(rec.indexOf("name_ls")).toString();
			long int x_coord=query.value(rec.indexOf("x_coordinates")).toInt();
			long int y_coord=query.value(rec.indexOf("y_coordinates")).toInt();
			QString signCode = query.value(rec.indexOf("excode_type_ls")).toString();
			QString shortNameLs = query.value(rec.indexOf("short_name_ls")).toString();
			QString idLs = query.value(rec.indexOf("id_ls")).toString();

			QList<Coord*> coordList;
			Coord *coord = new Coord(x_coord,y_coord);	
			coordList.append(coord);

			QMap<long int,QString> semantic_map;
			semantic_map[105] = shortNameLs;
			semantic_map[19] = shortNameLs;
			semantic_map[17501] = idLs;
			semantic_map[17502] = QString::number(FORMATIONS);

			SignData *signData = new SignData(signCode,coordList,semantic_map);
				
			formationsList.append(signData);
		}
	}
	return formationsList;
}



//==================================================================================
//==== Метод возвращает список объектов SignData с информацией =====================
//==== для нанесения на карту и инициализации условных знаков специальных условий ==
//==================================================================================
QList<SignData*> ViewManage::getSpecialConditions()
{
	QList<SignData*> conditionsList;

	QSqlQuery query;
	QString str=QString("SELECT special_conditions.name_special_conditions, special_conditions.semantika_1, \
						special_conditions.semantika_2 , type_special_conditions.excode_type_sc, \
						coordinates.x_coordinates, coordinates.y_coordinates, special_conditions.id_special_conditions FROM special_conditions, \
						region, type_special_conditions, coordinates, coord_spec_cond \
						where special_conditions.id_region=region.id_region AND \
						special_conditions.id_type_special_conditions=type_special_conditions.id_type_special_conditions \
						AND coord_spec_cond.id_special_conditions=special_conditions.id_special_conditions \
						AND coord_spec_cond.id_coordinates=coordinates.id_coordinates");
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{
			QString name_spec_cond = query.value(rec.indexOf("name_special_conditions")).toString();
			long int x_coord=query.value(rec.indexOf("x_coordinates")).toInt();
			long int y_coord=query.value(rec.indexOf("y_coordinates")).toInt();
			QString signCode = query.value(rec.indexOf("excode_type_sc")).toString();
			QString Sem_1_spec_cond = query.value(rec.indexOf("semantika_1")).toString();
			QString Sem_2_spec_cond = query.value(rec.indexOf("semantika_2")).toString();
			QString idSpecialConditions = query.value(rec.indexOf("id_special_conditions")).toString();

			QList<Coord*> coordList;
			Coord *coord = new Coord(x_coord,y_coord);	
			coordList.append(coord);

			QMap<long int,QString> semantic_map;
			semantic_map[17] = Sem_1_spec_cond;
			semantic_map[19] = Sem_2_spec_cond;
			semantic_map[17501] = idSpecialConditions;
			semantic_map[17502] = QString::number(SPECIAL_CONDITIONS);

			SignData *signData = new SignData(signCode,coordList,semantic_map);
				
			conditionsList.append(signData);
		}
	}
	return conditionsList;
}