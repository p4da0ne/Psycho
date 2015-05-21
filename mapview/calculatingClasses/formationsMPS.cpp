#include "formationsMPS.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <qmath.h>
#include <QSettings>
#include <my_mapaccess.h>
#include "mapview.h"

FormationsMPS::FormationsMPS(QObject *parent)
    : QObject(parent)
{
	//MainCodec = QTextCodec::codecForName("CP1251");
  	mpsCalculator = new calculating_mps;
	
	
	
}

FormationsMPS::~FormationsMPS()
{
    
}


//======================================================================================
//========= Метод перевода геодезических координат (WGS84) в прямоугольные =============
//======================================================================================
Coord* FormationsMPS::WGStoPlane(long int hMap,Coord *coordObject)
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


////============ Метод возвращает МПС формирований ==========================
////========  для вывода на карту знака с цифрами по ПСИХАМ =======================
////================================================================================
QList<SignData*> FormationsMPS::getFormationsMPS(long int hMap,double x1,double y1,double x2,double y2,bool enemyFlag)
{
	
	QList<SignData*> formationsMPSList;
	QSqlQuery query;
	
	QString enemyFl;
	if(enemyFlag) enemyFl = "true";
	else enemyFl = "false";
	
	QDate date;
	QString StrDate=date.currentDate().toString("dd.MM.yyyy");
	QString str=QString("SELECT coordinates.latitude_wgs_84_g,coordinates.latitude_wgs_84_m,coordinates.latitude_wgs_84_s, \
						coordinates.longitude_wgs_84_g,coordinates.longitude_wgs_84_m,coordinates.longitude_wgs_84_s, ls.id_ls, \
						ls.id_ls, coord_ls.id_ls \
						FROM ls, coordinates, coord_ls \
						WHERE coord_ls.id_coordinates=coordinates.id_coordinates \
						AND ls.id_ls = coord_ls.id_ls \
						AND enimy_ls = %1").arg(enemyFl);

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

			if(!((x_coord > x1) && (y_coord > y1) && (x_coord < x2) && (y_coord < y2))) continue;

			int idLs = query.value(rec.indexOf("id_ls")).toInt();
						
			
			float MPS = mpsCalculator->calculating(idLs);
			float MPS_kont = mpsCalculator->get_mps_kont();
			float MPS_ofec = mpsCalculator->get_mps_ofec();
			float MPS_priz = mpsCalculator->get_mps_priz();


			QString formationBelongs;
			QString kodeX;
			if(enemyFlag)
			{
				formationBelongs = "7";
				kodeX = "V00000000313";
			}
			else
			{
				formationBelongs = "6";
				kodeX = "V00000000312";
			}

			QList<Coord*> coordList;
			QMap<long int,QString> semantic_map;
			if (MPS!=-1)
			{
				
				// показатель подразделения
				semantic_map[60004]=QString::number(MPS);						
				// принадлежность
				semantic_map[60028]=formationBelongs;	
				semantic_map[60001]=QString::number(MPS_ofec);				
				semantic_map[60002]=QString::number(MPS_kont);	
				semantic_map[60003]=QString::number(MPS_priz);	

					
				// вид стрелки
				if (MPS < 0.3)					semantic_map[60012]="4";
				if ((MPS >= 0.3)&&(MPS <= 0.7))	semantic_map[60012]="5";
				if ((MPS > 0.7)&&(MPS <= 1.0))	semantic_map[60012]="3";
					
							
				
				Coord *coord = new Coord(x_coord,y_coord-6000);	
				coordList.append(coord);
			}
		
			SignData *signData = new SignData(kodeX,coordList,semantic_map);
				
			formationsMPSList.append(signData);
		}
	}
	return formationsMPSList;
}
