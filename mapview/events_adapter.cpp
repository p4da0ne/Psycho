#include "events_adapter.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QSettings>
#include <my_mapaccess.h>
#include "mapview.h"
#include <eventsmodel.h>

EventsAdapter::EventsAdapter(QDate *startPeriod,QDate *endPeriod,
				  QStandardItemModel *objectsModel,QStandardItemModel *statesModel,
				  QStandardItemModel *eventTypesModel,QObject *parent)
    : QObject(parent),objectsModel(objectsModel),statesModel(statesModel),
	  eventTypesModel(eventTypesModel),startPeriod(startPeriod),endPeriod(endPeriod)
{

	
}

EventsAdapter::EventsAdapter(QObject *parent)
    : QObject(parent)
{

	
}

EventsAdapter::~EventsAdapter()
{
    
}


//======================================================================================
//========= Метод перевода геодезических координат (WGS84) в прямоугольные =============
//======================================================================================
Coord* EventsAdapter::WGStoPlane(long int hMap,Coord *coordObject)
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
Coord* EventsAdapter::planeToWGS(long int hMap,Coord *coordObject)
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


//============================================================================
//======= Метод возвращает список статусов событий ===========================
//============================================================================
QStandardItemModel * EventsAdapter::getEventStatusList()
{
	QStandardItemModel *eventStatusList = new QStandardItemModel;

	QSqlQuery query;
	QString str=QString("SELECT id_event_status, name_event_status \
						 FROM event_status \
						 ORDER BY id_event_status");
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{
			int idStatus = query.value(rec.indexOf("id_event_status")).toInt();
			QString statusName = query.value(rec.indexOf("name_event_status")).toString();
		
			QStandardItem *stateItem = new QStandardItem;
			stateItem->setData(statusName,Qt::DisplayRole);
			stateItem->setData(idStatus,Qt::UserRole);
			stateItem->setCheckable(true);
			stateItem->setCheckState(Qt::Checked);
			eventStatusList->appendRow(stateItem);
		}
	}
	return eventStatusList;
}


//============================================================================
//======= Метод возвращает список типов событий ==============================
//============================================================================
QStandardItemModel * EventsMapModel::getEventTypesList()
{
	QStandardItemModel *eventTypesList = new QStandardItemModel;

	QSqlQuery query;
	QString str=QString("SELECT id_type_event, name_type_event \
						 FROM type_event \
						 ORDER BY id_type_event");
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{
			int idType = query.value(rec.indexOf("id_type_event")).toInt();
			QString typeName = query.value(rec.indexOf("name_type_event")).toString();
		
			QStandardItem *typeItem = new QStandardItem;
			typeItem->setData(typeName,Qt::DisplayRole);
			typeItem->setData(idType,Qt::UserRole);
			typeItem->setCheckable(true);
			typeItem->setCheckState(Qt::Checked);
			eventTypesList->appendRow(typeItem);
		}
	}
	return eventTypesList;
}


//================================================================================
//==== Метод возвращает список событий ===========================================
//==== для нанесения на карту и инициализации условных знаков событий ============
//================================================================================
QList<SignData*> EventsMapModel::getEvents(long int hMap,double x1,double y1,double x2,double y2)
{
	QList<SignData*> eventsList;

	QMultiMap<QString,int> objects;

	for(int row=0;row<objectsModel->rowCount();row++)
	{
		QModelIndex index = objectsModel->index(row,0);
		int objectType = objectsModel->data(index,Qt::UserRole+1).toInt();
		int idObject = objectsModel->data(index,Qt::UserRole).toInt();
		
		switch(objectType)
		{
			case FORMATIONS:
				objects.insert("ls",idObject);
				break;
			case REGIONS:
				objects.insert("region",idObject);
				break;
			case SPECIAL_CONDITIONS:
				objects.insert("special_conditions",idObject);
				break;
			case PERSONNEL:
				objects.insert("persones",idObject);
				break;
			case SMI_MEANS:
				objects.insert("mpo_pso",idObject);
				break;
			case FORMATIONS_MEANS:
				objects.insert("mpo_pso",idObject);
				break;
			case GROUPS_MEANS:
				objects.insert("mpo_pso",idObject);
				break;
		}
	}

	QStandardItemModel *eventsModel = new QStandardItemModel;
	
	//eventsModel = getEvents(
	
	
	
	
	///
	//QSqlQuery query;
	//QString str=QString("SELECT name_type_mpo_pso, coordinates.latitude_wgs_84_g,coordinates.latitude_wgs_84_m,coordinates.latitude_wgs_84_s,coordinates.longitude_wgs_84_g, \
	//					coordinates.longitude_wgs_84_m,coordinates.longitude_wgs_84_s, \
	//					type_mpo_pso.id_sign, mpo_pso.id_mpo_pso, mpo_pso.semantika_digit1, mpo_pso.semantika_digit2, mpo_pso.semantika_1,si.sign_key \
	//					FROM mpo_pso, coord_mpo_pso cmp, coordinates, type_mpo_pso, signs si \
	//					WHERE cmp.id_coordinates=coordinates.id_coordinates \
	//					AND mpo_pso.id_type_mpo_pso=type_mpo_pso.id_type_mpo_pso \
	//					AND mpo_pso.id_mpo_pso = cmp.id_mpo_pso \
	//					AND type_mpo_pso.id_sign = si.id_sign \
	//					AND type_mpo_pso.excode_type_mpo_pso <> '' \
	//					AND mpo_pso.id_smi > 0 ");
	//if(query.exec(str))
	//{
	//	QSqlRecord rec = query.record();
	//	while (query.next())
	//	{		
	//		int wgs_g = query.value(rec.indexOf("latitude_wgs_84_g")).toInt();
	//		int wgs_m = query.value(rec.indexOf("latitude_wgs_84_m")).toInt();
	//		double wgs_s = query.value(rec.indexOf("latitude_wgs_84_s")).toDouble();
	//		int long_wgs_g = query.value(rec.indexOf("longitude_wgs_84_g")).toInt();
	//		int long_wgs_m = query.value(rec.indexOf("longitude_wgs_84_m")).toInt();
	//		double long_wgs_s = query.value(rec.indexOf("longitude_wgs_84_s")).toDouble();
	//				
	//		///получить из запроса 6 параметров координат WGS

	//		Coord c1(wgs_g,wgs_m,wgs_s,long_wgs_g,long_wgs_m,long_wgs_s);
	//		
	//		Coord *c2 = WGStoPlane(hMap,&c1);		
	//		
	//		double x_coord = c2->getX();
	//		double y_coord = c2->getY();

	//		
	//		if(!((x_coord > x1) && (y_coord > y1) && (x_coord < x2) && (y_coord < y2))) continue;	
	//		
	//		QString name_type_mpo_pso = query.value(rec.indexOf("name_type_mpo_pso")).toString();
	//		QString signCode = query.value(rec.indexOf("sign_key")).toString();
	//		QString id_mpo_pso = query.value(rec.indexOf("id_mpo_pso")).toString();

	//		// дальность (радиус) действия, километры
	//		QString semantika_digit1_mpo_pso = query.value(rec.indexOf("semantika_digit1")).toString();
	//		// угол (направление) относительно горизонта против часовой стрелки, градусы
	//		QString semantika_digit2_mpo_pso = query.value(rec.indexOf("semantika_digit2")).toString();
	//		QString semantika_1_mpo_pso = query.value(rec.indexOf("semantika_1")).toString();

	//		QList<Coord*> coordList;
	//		Coord *coord = new Coord(x_coord,y_coord);	
	//		coordList.append(coord);
	//		
	//		///////////////////////////////////
	//		double radius;
	//		double angle;
	//		
	//		if(semantika_digit1_mpo_pso > 0)
	//		{
	//			radius = semantika_digit1_mpo_pso.toDouble()*250;
	//		}
	//		if(semantika_digit2_mpo_pso > 0)
	//		{
	//			angle = 5*3.14/2 - (semantika_digit2_mpo_pso.toDouble()*3.14/180);
	//		}

	//	 
	//		// если зачек радио-теле центра (свой или вражеский), то добавляем вторую метрику
	//		if ((signCode=="V0000169007")||(signCode=="V0000169029"))
	//		{	
	//			coord = new Coord(x_coord,y_coord+radius);
	//			coordList.append(coord);
	//		}


	//		if (signCode=="L00000060504") // самолет
	//		{	
	//			coord = new Coord(x_coord+120000,y_coord+140000);
	//			coordList.append(coord);
	//			coord = new Coord(x_coord+120000+70000,y_coord+140000-80000);
	//			coordList.append(coord);
	//		}


	//		// если передвижная звуковещательная станция, то добавляем вторую метрику 
	//		// (получаем ее как угол места и длину радиус-вектора, направленного из первой точки метрики)
	//		if (signCode=="V0000060505")
	//		{	
	//			double xx=4*radius*qCos(angle);
	//			double yy=4*radius*qSin(angle);
	//			
	//			coord = new Coord(x_coord+xx,y_coord+yy);
	//			coordList.append(coord);
	//		}
	//		
	//		
	//		QMap<long int,QString> semantic_map;

	//		semantic_map[17501] = id_mpo_pso;
	//		semantic_map[17502] = QString::number(SMI_MEANS);
	//		semantic_map[18]=semantika_digit1_mpo_pso;	// иногда это наполнение значка (в тех случаях, когда не "дальность")
	//		semantic_map[19]=semantika_1_mpo_pso;	// подпись значка
	//		semantic_map[32811]=semantika_digit1_mpo_pso;	//дальность действия средства
	//		semantic_map[32852]=semantika_digit2_mpo_pso;	//направление (угол) действия средства

	//		SignData *signData = new SignData(signCode,coordList,semantic_map);
	//			
	//		smiMeansList.append(signData);
	//	}
	//}

	return eventsList;
}