#include "regions_mpos.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <qmath.h>
#include <QSettings>
#include <my_mapaccess.h>
#include "mapview.h"

RegionsMpos::RegionsMpos(QObject *parent)
    : QObject(parent)
{
	//MainCodec = QTextCodec::codecForName("CP1251");
  	regionCalculator = new Calculate_K_omkrf;
	
	
	
}

RegionsMpos::~RegionsMpos()
{
    
}


//======================================================================================
//========= Метод перевода геодезических координат (WGS84) в прямоугольные =============
//======================================================================================
Coord* RegionsMpos::WGStoPlane(long int hMap,Coord *coordObject)
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
QList<SignData*> RegionsMpos::getRegions(long int hMap,double x1,double y1,double x2,double y2)
{
	QList<SignData*> regionsList;
///
	QSqlQuery query;
	int idRegion;
	
	QList<int> regionsIdList;

	query.exec(QString("SELECT id_region from region ORDER BY id_region"));
	while (query.next())
	{ 
		idRegion = query.value(0).toInt();
		regionsIdList.append(idRegion);
	}
	

	//======== ProgressDialog =======
	QProgressDialog progress("Вычисление МПОС регионов...", "Прервать", 0,regionsIdList.count());
	progress.setWindowModality(Qt::WindowModal);

	//===============================
	for(int i=0;i<regionsIdList.count();i++)
	{
		progress.setValue(i);
		if(progress.wasCanceled())
			break;

		if(isRegionOnMap(hMap,regionsIdList.at(i),x1,y1,x2,y2))
		{
			//----- Код условного знака региона  -----
			QString exCode = getRegionExCode(regionsIdList.at(i));
			//----- Метрика региона (список координат) ---------
			QList<Coord*> coordList = getRegionMetric(hMap,regionsIdList.at(i));

			//----- Семантики региона ---------
			QMap<long int,QString> semantic_map = getRegionSemantics(regionsIdList.at(i));
		
			SignData *signData = new SignData(exCode,coordList,semantic_map);

			regionsList.append(signData);
		}
	}
	progress.setValue(regionsIdList.count());
	return regionsList;
}


//=============================================================================================
//===== Метод проверки попадает ли регион на карту ==========
//=============================================================================================
bool RegionsMpos::isRegionOnMap(long int hMap,int idRegion,double x1,double y1,double x2,double y2)
{
	QSqlQuery query;
	
	QString str=QString("SELECT c.latitude_wgs_84_g, c.latitude_wgs_84_m, c.latitude_wgs_84_s, \
								c.longitude_wgs_84_g, c.longitude_wgs_84_m, c.longitude_wgs_84_s \
						 FROM coord_region c_r, coordinates c \
						 WHERE c_r.id_coordinates = c.id_coordinates \
						 AND c_r.id_region = %1").arg(idRegion);
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

			Coord regionCoordinates(wgs_g,wgs_m,wgs_s,long_wgs_g,long_wgs_m,long_wgs_s);
			
			Coord *plainRegionCoordinates = WGStoPlane(hMap,&regionCoordinates);		
			
			double x_coord = plainRegionCoordinates->getX();
			double y_coord = plainRegionCoordinates->getY();

			//если хоть одна координата региона попадает в область карты, то выходим с true
			if(((x_coord > x1) && (y_coord > y1) && (x_coord < x2) && (y_coord < y2))) return true;
		}
		return false;
	}
	return false;
}

//=======================================================================
//====== Метод получения кода условного знака региона по его ID =========
//=======================================================================
QString RegionsMpos::getRegionExCode(int idRegion)
{
	QSqlQuery query;
	QString exCode;

	QString str=QString("SELECT s.sign_key \
						 FROM region r, type_region t, signs s \
						 WHERE r.id_type_region=t.id_type_region \
						 AND t.id_sign = s.id_sign \
						 AND r.id_region = %1").arg(idRegion);
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		query.next();
		
		exCode = query.value(rec.indexOf("sign_key")).toString();
	}
	return exCode;
}


//=======================================================================
//====== Метод возвращает метрику региона по его ID =====================
//=======================================================================
QList<Coord*> RegionsMpos::getRegionMetric(long int hMap,int idRegion)
{
	QSqlQuery query;
	QList<Coord*> coordList;


	QString str=QString("SELECT c.latitude_wgs_84_g, c.latitude_wgs_84_m, c.latitude_wgs_84_s, \
								c.longitude_wgs_84_g, c.longitude_wgs_84_m, c.longitude_wgs_84_s \
						 FROM coord_region c_r, coordinates c \
						 WHERE c_r.id_coordinates = c.id_coordinates \
						 AND c_r.id_region = %1 \
						 ORDER BY c.id_coordinates").arg(idRegion);
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

			Coord regionCoordinates(wgs_g,wgs_m,wgs_s,long_wgs_g,long_wgs_m,long_wgs_s);
			
			Coord *plainRegionCoordinates = WGStoPlane(hMap,&regionCoordinates);		
			
			coordList.append(plainRegionCoordinates);			
		}
	}
	return coordList;	
}

//=======================================================================
//====== Метод возвращает семантики региона по его ID ======================
//=======================================================================
QMap<long int,QString> RegionsMpos::getRegionSemantics(int idRegion)
{
	QMap<long int,QString> semantic_map;

	semantic_map[17501] = QString::number(idRegion);
			semantic_map[17502] = QString::number(REGIONS);

			float rez = regionCalculator->get_Rez_on_id_region(idRegion);
			
			QString mpos;
			if(rez > 0 && rez < 0.3)
				mpos="1";
			if(rez >= 0.3 && rez < 0.5)
				mpos="2";
			if(rez >=0.5 && rez <= 0.8)
				mpos="3";
			if(rez == 0)
				mpos="4";

			semantic_map[60011] = mpos;

	return semantic_map;
}