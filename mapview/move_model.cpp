#include "move_model.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <qmath.h>
#include <QSettings>
#include <my_mapaccess.h>
#include "mapview.h"

MoveModel::MoveModel(QObject *parent)
    : QObject(parent)
{
	//MainCodec = QTextCodec::codecForName("CP1251");
  	
	
	
	
}

MoveModel::~MoveModel()
{
    
}


//======================================================================================
//========= Метод перевода геодезических координат (WGS84) в прямоугольные =============
//======================================================================================
Coord* MoveModel::WGStoPlane(long int hMap,Coord *coordObject)
{
   Coord *tempCoord = coordObject;
   
   if(hMap == 0) return 0;

	MyMapAccess *map = new MyMapAccess;

   if(map->mapIsGeoSupported(hMap))
    {
        GEODEGREE N, E;
        double N_rad, E_rad, H;

		N.Degree = tempCoord->getLatDegrees();
		N.Minute = tempCoord->getLatMinutes();
		N.Second = tempCoord->getLatSeconds();

        map->mapDegreeToRadian(&N, &N_rad);

		E.Degree = tempCoord->getLongDegrees();
		E.Minute = tempCoord->getLongMinutes();
		E.Second = tempCoord->getLongSeconds();

        map->mapDegreeToRadian(&E, &E_rad);

        map->mapGeoWGS84ToPlane3D(hMap,&N_rad,&E_rad,&H);

        tempCoord->setX(N_rad);
		tempCoord->setY(E_rad);
		tempCoord->setH(H);
    }
	return tempCoord;
}

//======================================================================================
//========= Метод перевода прямоугольных координат в геодезические (WGS-84) ============
//======================================================================================
Coord* MoveModel::planeToWGS(long int hMap,Coord *coordObject)
{
	Coord *tempCoord = coordObject;
   
	if(hMap == 0) return 0;

	MyMapAccess *map = new MyMapAccess;
	int nD,nM,eD,eM;
	double nS,eS;
	
	if(map->mapIsGeoSupported(hMap))
    {
        GEODEGREE N, E;
		double N_rad, E_rad, H;
		N_rad = tempCoord->getX();
		E_rad = tempCoord->getY();
		map->mapPlaneToGeoWGS843D(hMap,&N_rad,&E_rad,&H);
		map->mapRadianToDegree(&N_rad,&N);
		map->mapRadianToDegree(&E_rad,&E);
	    nD = N.Degree;	 
		nM = N.Minute;
		nS = N.Second;
		eD = E.Degree;
		eM = E.Minute;
		eS = E.Second;
		
		tempCoord->setLatitude(nD,nM,nS);
		tempCoord->setLongitude(eD,eM,eS);
    }
	return tempCoord;
}

	
//===================================================================================
//==== Метод возвращает список координат объекта в зависимости от его типа. =========
//==== Прямоугольные координаты формируются из WGS84 в проекции карты ===============
//===================================================================================
QList<Coord *> MoveModel::getObjectCoordinates(long int hMap, int idObject, int objectType)
{
	QList<Coord *> coordinates;

	QSqlQuery query;
	QString str;

	switch(objectType)
		{
			case FORMATIONS:
				str = QString("SELECT cs.latitude_wgs_84_g,cs.latitude_wgs_84_m,cs.latitude_wgs_84_s,cs.longitude_wgs_84_g, \
							   cs.longitude_wgs_84_m,cs.longitude_wgs_84_s \
							   FROM ls, coord_ls c_l, coordinates cs \
							   WHERE ls.id_ls = c_l.id_ls \
							   AND c_l.id_coordinates=cs.id_coordinates \
							   AND ls.id_ls = %1").arg(idObject);
				break;
			
			case SPECIAL_CONDITIONS:
				str = QString("SELECT cs.latitude_wgs_84_g,cs.latitude_wgs_84_m,cs.latitude_wgs_84_s,cs.longitude_wgs_84_g, \
							   cs.longitude_wgs_84_m,cs.longitude_wgs_84_s \
							   FROM special_conditions s_c, coord_spec_cond c_sc, coordinates cs \
							   WHERE s_c.id_special_conditions = c_sc.id_special_conditions \
							   AND c_sc.id_coordinates=cs.id_coordinates \
							   AND s_c.id_special_conditions = %1").arg(idObject);
				break;
			case SMI_MEANS:
				str = QString("SELECT cs.latitude_wgs_84_g,cs.latitude_wgs_84_m,cs.latitude_wgs_84_s,cs.longitude_wgs_84_g, \
							   cs.longitude_wgs_84_m,cs.longitude_wgs_84_s \
							   FROM mpo_pso mp, coord_mpo_pso c_mp, coordinates cs \
							   WHERE mp.id_mpo_pso = c_mp.id_mpo_pso \
							   AND c_mp.id_coordinates=cs.id_coordinates \
							   AND mp.id_mpo_pso = %1").arg(idObject);
				break;
			
			case FORMATIONS_MEANS:
				str = QString("SELECT cs.latitude_wgs_84_g,cs.latitude_wgs_84_m,cs.latitude_wgs_84_s,cs.longitude_wgs_84_g, \
							   cs.longitude_wgs_84_m,cs.longitude_wgs_84_s \
							   FROM mpo_pso mp, coord_mpo_pso c_mp, coordinates cs \
							   WHERE mp.id_mpo_pso = c_mp.id_mpo_pso \
							   AND c_mp.id_coordinates=cs.id_coordinates \
							   AND mp.id_mpo_pso = %1").arg(idObject);
				break;
							
			case GROUPS_MEANS:
				str = QString("SELECT cs.latitude_wgs_84_g,cs.latitude_wgs_84_m,cs.latitude_wgs_84_s,cs.longitude_wgs_84_g, \
							   cs.longitude_wgs_84_m,cs.longitude_wgs_84_s \
							   FROM mpo_pso mp, coord_mpo_pso c_mp, coordinates cs \
							   WHERE mp.id_mpo_pso = c_mp.id_mpo_pso \
							   AND c_mp.id_coordinates=cs.id_coordinates \
							   AND mp.id_mpo_pso = %1").arg(idObject);
				break;
			
			case REGIONS:
				str = QString("SELECT cs.latitude_wgs_84_g,cs.latitude_wgs_84_m,cs.latitude_wgs_84_s,cs.longitude_wgs_84_g, \
							   cs.longitude_wgs_84_m,cs.longitude_wgs_84_s \
							   FROM region r, coord_region c_r, coordinates cs \
							   WHERE r.id_region = c_r.id_region \
							   AND c_r.id_coordinates=cs.id_coordinates \
							   AND r.id_region = %1").arg(idObject);
				break;

			case PERSONNEL:
				str = QString("SELECT cs.latitude_wgs_84_g,cs.latitude_wgs_84_m,cs.latitude_wgs_84_s,cs.longitude_wgs_84_g, \
							   cs.longitude_wgs_84_m,cs.longitude_wgs_84_s \
							   FROM persones ps, coord_persones c_p, coordinates cs \
							   WHERE ps.id_persones = c_p.id_persones \
							   AND c_p.id_coordinates=cs.id_coordinates \
							   AND ps.id_persones = %1").arg(idObject);
			break;
			
			case EVENTS:
				str = QString("SELECT cs.latitude_wgs_84_g,cs.latitude_wgs_84_m,cs.latitude_wgs_84_s,cs.longitude_wgs_84_g, \
							   cs.longitude_wgs_84_m,cs.longitude_wgs_84_s \
							   FROM events e, coord_events c_e, coordinates cs \
							   WHERE e.id_event = c_e.id_event \
							   AND c_e.id_coordinates=cs.id_coordinates \
							   AND e.id_event = %1").arg(idObject);
				break;
		}
	
	
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
			
			Coord *c2 = new Coord(*WGStoPlane(hMap,&c1));		

			coordinates.append(c2);
		}
	}

	return coordinates;
}




//==================================================================================================
//================ Метод обновления координат объекта в БД (для одной координаты) ==================
//==================================================================================================
void MoveModel::updateObjectCoordinates(int idObject, int objectType, Coord *coord)
{
	switch(objectType)
		{
			case FORMATIONS:
				updateFormationCoordinates(idObject, coord);
				break;
			
			case SPECIAL_CONDITIONS:
				updateSpecialConditionsCoordinates(idObject, coord);
				break;
			case SMI_MEANS:
				updateMeansCoordinates(idObject, coord);
				break;
			
			case FORMATIONS_MEANS:
				updateMeansCoordinates(idObject, coord);
				break;
							
			case GROUPS_MEANS:
				updateMeansCoordinates(idObject, coord);
				break;
			
			case PERSONNEL:
				updatePersonesCoordinates(idObject, coord);
				break;

			case EVENTS:
				updateEventsCoordinates(idObject, coord);
				break;
		}
	
	

}


//============================================================================
//======= Метод обновления координат формирования ===========
//============================================================================
bool MoveModel::updateFormationCoordinates(int idLs, Coord *coord)
{
	QSqlQuery query;
	QString str;

	str = QString("SELECT cs.id_coordinates \
				   FROM coordinates cs, ls, coord_ls c_l \
				   WHERE ls.id_ls=c_l.id_ls \
				   AND c_l.id_coordinates = cs.id_coordinates \
				   AND ls.id_ls = %1").arg(idLs);
	
	if(query.exec(str))
	{
		query.next();
		int idCoordinates = query.value(0).toInt();

		if(updateCoordinates(idCoordinates,coord))
		{
			return true;
		}
	}
	return false;
}


//============================================================================
//======= Метод обновления координат персоналии ===========
//============================================================================
bool MoveModel::updatePersonesCoordinates(int idPersone, Coord *coord)
{
	QSqlQuery query;
	QString str;

	str = QString("SELECT cs.id_coordinates \
					FROM coordinates cs, persones ps, coord_persones c_p \
					WHERE ps.id_persones=c_p.id_persones \
					AND c_p.id_coordinates = cs.id_coordinates \
					AND ps.id_persones = %1").arg(idPersone);
	
	if(query.exec(str))
	{
		query.next();
		int idCoordinates = query.value(0).toInt();

		if(updateCoordinates(idCoordinates,coord))
		{
			return true;
		}
	}
	return false;
}


//============================================================================
//======= Метод обновления координат средства ===========
//============================================================================
bool MoveModel::updateMeansCoordinates(int idMeans, Coord *coord)
{
	QSqlQuery query;
	QString str;

	str = QString("SELECT cs.id_coordinates \
					FROM coordinates cs, mpo_pso m_p, coord_mpo_pso c_m_p \
					WHERE m_p.id_mpo_pso=c_m_p.id_mpo_pso \
					AND c_m_p.id_coordinates = cs.id_coordinates \
					AND m_p.id_mpo_pso = %1").arg(idMeans);
	
	if(query.exec(str))
	{
		query.next();
		int idCoordinates = query.value(0).toInt();

		if(updateCoordinates(idCoordinates,coord))
		{
			return true;
		}
	}
	return false;
}


//============================================================================
//======= Метод обновления координат особых условий ===========
//============================================================================
bool MoveModel::updateSpecialConditionsCoordinates(int idSpecCond, Coord *coord)
{
	QSqlQuery query;
	QString str;

	str = QString("SELECT cs.id_coordinates \
					FROM coordinates cs, special_conditions s_c, coord_spec_cond c_s_c \
					WHERE s_c.id_special_conditions=c_s_c.id_special_conditions \
					AND c_s_c.id_coordinates = cs.id_coordinates \
					AND s_c.id_special_conditions = %1").arg(idSpecCond);
	
	if(query.exec(str))
	{
		query.next();
		int idCoordinates = query.value(0).toInt();

		if(updateCoordinates(idCoordinates,coord))
		{
			return true;
		}
	}
	return false;
}

//============================================================================
//======= Метод обновления координат событий ===========
//============================================================================
bool MoveModel::updateEventsCoordinates(int idEvent, Coord *coord)
{
	QSqlQuery query;
	QString str;

	str = QString("SELECT cs.id_coordinates \
					FROM coordinates cs, events e, coord_events c_e \
					WHERE e.id_event=c_e.id_event \
					AND c_e.id_coordinates = cs.id_coordinates \
					AND e.id_event = %1").arg(idEvent);
	
	if(query.exec(str))
	{
		query.next();
		int idCoordinates = query.value(0).toInt();

		if(updateCoordinates(idCoordinates,coord))
		{
			return true;
		}
	}
	return false;
}

//==============================================================================
//==== Метод обновления координат в таблице по idCoordinates ===================
//==============================================================================
bool MoveModel::updateCoordinates(int idCoordinates, Coord *coord)
{
	QSqlQuery query;
	QString str;

	
	query.prepare("UPDATE coordinates SET latitude_wgs_84_g = ?,latitude_wgs_84_m = ?,latitude_wgs_84_s = ?, \
				   longitude_wgs_84_g = ?,longitude_wgs_84_m = ?,longitude_wgs_84_s = ? \
                   WHERE id_coordinates = ?");
	query.addBindValue(coord->getLatDegrees());
	query.addBindValue(coord->getLatMinutes());
	query.addBindValue(coord->getLatSeconds());
	query.addBindValue(coord->getLongDegrees());
	query.addBindValue(coord->getLongMinutes());
	query.addBindValue(coord->getLongSeconds());
	query.addBindValue(idCoordinates);

	if(query.exec())
	{
		return true;
	}
	str = query.lastError().text();
	return false;
}

