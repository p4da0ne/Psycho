#include "events_map_model.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QSettings>
#include <my_mapaccess.h>
#include "mapview.h"
#include <eventsmodel.h>

EventsMapModel::EventsMapModel(QDate *startPeriod,QDate *endPeriod,
				  QStandardItemModel *objectsModel,QStandardItemModel *statesModel,
				  QStandardItemModel *eventTypesModel,QObject *parent)
    : QObject(parent),objectsModel(objectsModel),statesModel(statesModel),
	  eventTypesModel(eventTypesModel),startPeriod(startPeriod),endPeriod(endPeriod)
{

	
}

EventsMapModel::EventsMapModel(QObject *parent)
    : QObject(parent)
{

	
}

EventsMapModel::~EventsMapModel()
{
    
}


//======================================================================================
//========= Метод перевода геодезических координат (WGS84) в прямоугольные =============
//======================================================================================
Coord* EventsMapModel::WGStoPlane(long int hMap,Coord *coordObject)
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
Coord* EventsMapModel::planeToWGS(long int hMap,Coord *coordObject)
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
QStandardItemModel * EventsMapModel::getEventStatusList()
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

	QStandardItemModel *eventsModel = new QStandardItemModel;
	
	QList<int> eventIdList = getIdEventsByFilter();  //возвращает список id событий в соответствии с фильтром

	for(int i=0;i<eventIdList.count();i++)
	{
		//генерируем запрос на выборку информации о событии по его id
		QString queryString = createEventsFilterQuery(eventIdList.at(i)); 
		QSqlQuery query;

		if(query.exec(queryString))
		{
			QSqlRecord rec = query.record();
			while (query.next())
			{		
				int idEvent = eventIdList.at(i);
				int idTypeEvent = query.value(rec.indexOf("id_type_event")).toInt();
				int idObject = query.value(rec.indexOf("id_object")).toInt();
				bool isObjectEventSource = query.value(rec.indexOf("is_events_source")).toBool();
				QString objectTableName = query.value(rec.indexOf("table_name")).toString(); 
				//------ проверка, попадает ли событие на карту ------
				bool isOnMap = isEventOnMap(idEvent,hMap,x1,y1,x2,y2);			
				if(isOnMap) //если попадает, то формируем для него условный знак
				{
					QMap<long int,QString> semantic_map;

					semantic_map[17501] = QString::number(idEvent);
					semantic_map[17502] = QString::number(EVENTS);
					semantic_map[60030] = QString::number(getEventStatusId(idEvent));	
				
					if(isObjectEventSource)
					{
						semantic_map[19] = getObjectShortName(idObject, objectTableName);	// сокращенное наименование инициатора события (например п/д, осуществляющее ИТВ или др.)
					}
					else
					{
						semantic_map[218] = getObjectShortName(idObject, objectTableName); //номер (название) цели
					}
					//semantic_map[17] = ;	//дата и время события
				
						
				
				
					QString signCode = getEventSignCode(idTypeEvent);


				

					QList<Coord*> eventMetric = getEventCoordinates(hMap,idEvent);
					QList<Coord*> objectMetric = getObjectCoordinates(hMap,idObject, objectTableName);
				
					//----- Проверка равенства координат события и объекта -------
					bool coordEqualFlag = false;
					if(eventMetric.count() == objectMetric.count())
					{
						for(int i=0;i<eventMetric.count();i++)
						{
							if(eventMetric.at(i) == objectMetric.at(i))
							{
								coordEqualFlag = true;
							}
							else
							{
								coordEqualFlag = false;
							}
						}
					}
					//--------------------------------------------------------------
					if(coordEqualFlag) //если координаты события совпадают с координатами объекта, рисуем знак рядом с объектом
					{
				
					}
					else //если координаты события не совпадают с координатами объекта, то рисуем знак в этой точке (с координатами события)
					{		
					
						SignData *signData = new SignData(signCode,eventMetric,semantic_map);
						
						eventsList.append(signData);
					}

				}

		}
	}
	return eventsList;
	}
}


//=============================================================================================
//==== Метод проверки попадает ли событие на текущую карту ====================================
//=============================================================================================
bool EventsMapModel::isEventOnMap(const int idEvent,long int hMap,double x1,double y1,double x2,double y2)
{
	QList<Coord*> coordList = getEventCoordinates(hMap,idEvent);

	for(int i=0;i<coordList.count();i++)
	{
		Coord *coord = WGStoPlane(hMap,coordList.at(i));			
		double x_coord = coord->getX();
		double y_coord = coord->getY();	
		if(((x_coord > x1) && (y_coord > y1) && (x_coord < x2) && (y_coord < y2)))
		{
			return true;
		}
	}
	return false;
}

//======================================================================================
//======== Метод возвращает код знака события по идентификатору типа события ===========
//======================================================================================
QString EventsMapModel::getEventSignCode(const int idTypeEvent)
{
	QString signCode;
	QSqlQuery query;
	QString str=QString("SELECT s.sign_key \
						 FROM type_event t_e, signs s \
						 WHERE t_e.id_sign = s.id_sign \
						 AND t_e.id_type_event = %1").arg(idTypeEvent);
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		query.next();
		signCode = query.value(rec.indexOf("sign_key")).toString();
	}
	return signCode;
}

//======================================================================================
//======== Метод возвращает идентификатор статуса события ==============================
//======================================================================================
int EventsMapModel::getEventStatusId(int idEvent)
{
	int idStatus;
	QSqlQuery query;
	QString str=QString("SELECT e.id_event_status FROM events e WHERE e.id_event = %1").arg(idEvent);
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		query.next();
		idStatus = query.value(rec.indexOf("id_event_status")).toInt();
	}
	return idStatus;
}

//=============================================================================================
//==== метод возвращает список координат события (по умолчанию в списке одна координата) ======
//=============================================================================================
QList<Coord*> EventsMapModel::getEventCoordinates(long int hMap,const int idEvent)
{
	QList<Coord*> coordList;
	
	QSqlQuery query;
	QString str=QString("SELECT cc.latitude_wgs_84_g,cc.latitude_wgs_84_m,cc.latitude_wgs_84_s,cc.longitude_wgs_84_g, \
						 cc.longitude_wgs_84_m,cc.longitude_wgs_84_s \
						 FROM coord_events c_e, coordinates cc \
						 WHERE c_e.id_coordinates = cc.id_coordinates \
						 AND c_e.id_event = %1").arg(idEvent);
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
					
			Coord *coord = new Coord(wgs_g,wgs_m,wgs_s,long_wgs_g,long_wgs_m,long_wgs_s);
			coord = WGStoPlane(hMap,coord);
			coordList.append(coord);
		}
	}
	return coordList;
}


//=======================================================================
//====== Метод возвращает список координат объекта =====================
//=======================================================================
QList<Coord*> EventsMapModel::getObjectCoordinates(long int hMap,int idObject, QString tableName)
{
	QSqlQuery query;
	QList<Coord*> coordList;

	QString queryStr = "SELECT c.latitude_wgs_84_g, c.latitude_wgs_84_m, c.latitude_wgs_84_s, \
								c.longitude_wgs_84_g, c.longitude_wgs_84_m, c.longitude_wgs_84_s ";

	QString fromStr, whereStr, andStr;

	if(tableName == "persones")
	{
		fromStr = "FROM coordinates c, coord_persones ";
		whereStr = "WHERE coord_persones.id_coordinates = c.id_coordinates ";
		andStr = "AND coord_persones.id_persones = %1 ";
	}
	if(tableName == "ls")
	{
		fromStr = "FROM coordinates c, coord_ls ";
		whereStr = "WHERE coord_ls.id_coordinates = c.id_coordinates ";
		andStr = "AND coord_ls.id_ls = %1 ";
	}
	if(tableName == "mpo_pso")
	{
		fromStr = "FROM coordinates c, coord_mpo_pso ";
		whereStr = "WHERE coord_mpo_pso.id_coordinates = c.id_coordinates ";
		andStr = "AND coord_mpo_pso.id_mpo_pso = %1 ";
	}
	if(tableName == "region")
	{
		fromStr = "FROM coordinates c, coord_region ";
		whereStr = "WHERE coord_region.id_coordinates = c.id_coordinates ";
		andStr = "AND coord_region.id_region = %1 ";
	}
	if(tableName == "special_conditions")
	{
		fromStr = "FROM coordinates c, coord_special_conditions ";
		whereStr = "WHERE coord_special_conditions.id_coordinates = c.id_coordinates ";
		andStr = "AND coord_special_conditions.id_special_conditions = %1 ";
	}
	
	queryStr.append(fromStr);
	queryStr.append(whereStr);
	queryStr.append(QString(andStr).arg(idObject));
	queryStr.append(QString("ORDER BY c.id_coordinates"));

	if(query.exec(queryStr))
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

			Coord *objectCoordinates = new Coord(wgs_g,wgs_m,wgs_s,long_wgs_g,long_wgs_m,long_wgs_s);
			objectCoordinates = WGStoPlane(hMap,objectCoordinates);
			coordList.append(objectCoordinates);			
		}
	}
	return coordList;	
}


//=======================================================================
//====== Метод возвращает сокращенное наименование объекта =====================
//=======================================================================
QString EventsMapModel::getObjectShortName(int idObject,QString tableName)
{
	QSqlQuery query;
	QString objectName;

	QString queryStr;

	if(tableName == "persones")
	{
		queryStr = QString("SELECT name_persones FROM persones WHERE id_persones = %1").arg(idObject);
	}
	if(tableName == "ls")
	{
		queryStr = QString("SELECT short_name_ls FROM ls WHERE id_ls = %1").arg(idObject);
	}
	if(tableName == "mpo_pso")
	{
		queryStr = QString("SELECT name_mpo_pso FROM mpo_pso WHERE id_mpo_pso = %1").arg(idObject);
	}
	if(tableName == "region")
	{
		queryStr = QString("SELECT name_region FROM region WHERE id_region = %1").arg(idObject);
	}
	if(tableName == "special_conditions")
	{
		queryStr = QString("SELECT name_special_conditions FROM special_conditions WHERE id_special_conditions = %1").arg(idObject);
	}
	
	if(query.exec(queryStr))
	{
		while(query.next())
		{		
			objectName = query.value(0).toString();	
		}
	}
	return objectName;	
}

//==============================================================================================
//== Метод формирует строку запроса в БД для поиска событий в соответствии с фильтром ==========
//== Параметры фильтрации инициализируются в конструкторе при создании объекта модели событий ==
//==============================================================================================
QString EventsMapModel::createEventsFilterQuery(int idEvent)
{
	QString queryStr;

	queryStr.append("SELECT e.id_type_event, e_o.id_object, e_o.is_events_source, t_e.table_name \
					 FROM events e, event_objects e_o, type_event_object t_e, type_event tt	\
					 WHERE e.id_event = e_o.id_event \
					 AND e.id_type_event = tt.id_type_event \
					 AND e_o.id_type_event_object=t_e.id_type_event_object ");

	if(startPeriod->isValid() && endPeriod->isValid())
	{
		QDateTime startDateTime(*startPeriod,QTime(0,0,0));
		QDateTime endDateTime(*endPeriod,QTime(23,59,59));
	
		QString startDateT = startDateTime.toString("yyyy-MM-dd hh:mm:ss");
		QString endDateT = endDateTime.toString("yyyy-MM-dd hh:mm:ss");
		queryStr.append(QString("AND (e.time_event_start <= '%2' AND e.time_event_end >= '%1') ").arg(startDateT).arg(endDateT));
	}
	else
	{
		return queryStr;
	}
	//------- статусы событий --------
	int checkedStatusCount = checkedItemsCount(*statesModel);
	if(checkedStatusCount > 0)
	{
		QString orStr;
		bool firstOrFlag = true;
		for(int row=0;row<statesModel->rowCount();row++)
		{
			if(statesModel->item(row,0)->checkState() == Qt::Checked)
			{
				int idStatus = statesModel->data(statesModel->index(row,0),Qt::UserRole).toInt();
				if(firstOrFlag)
				{
					orStr.append(QString("e.id_event_status = %1 ").arg(idStatus));
					firstOrFlag = false;
				}
				else
				{
					orStr.append(QString("OR e.id_event_status = %1 ").arg(idStatus));
				}
			}
		}
		queryStr.append(QString("AND (%1) ").arg(orStr));
	}
	//------- типы событий --------
	int checkedTypesCount = checkedItemsCount(*eventTypesModel);
	if(checkedTypesCount > 0)
	{
		QString orStr;
		bool firstOrFlag = true;
		for(int row=0;row<eventTypesModel->rowCount();row++)
		{
			if(eventTypesModel->item(row,0)->checkState() == Qt::Checked)
			{
				int idType = eventTypesModel->data(eventTypesModel->index(row,0),Qt::UserRole).toInt();
				if(firstOrFlag)
				{
					orStr.append(QString("e.id_type_event = %1 ").arg(idType));
					firstOrFlag = false;
				}
				else
				{
					orStr.append(QString("OR e.id_type_event = %1 ").arg(idType));
				}
			}
		}
		queryStr.append(QString("AND (%1) ").arg(orStr));
	}
	//------- объекты, с которыми связаны события --------
	if(objectsModel->rowCount() > 0)
	{
		QString orStr;
		bool firstOrFlag = true;
		for(int row=0;row<objectsModel->rowCount();row++)
		{
			int idObject = objectsModel->data(objectsModel->index(row,0),Qt::UserRole).toInt();
			if(firstOrFlag)
			{
				orStr.append(QString("e_o.id_object = %1 ").arg(idObject));
				firstOrFlag = false;
			}
			else
			{
				orStr.append(QString("OR e_o.id_object = %1 ").arg(idObject));
			}
		}
		queryStr.append(QString("AND (%1) ").arg(orStr));
	}
	queryStr.append(QString("AND e.id_event = %1").arg(idEvent));

	return queryStr;
}

//====================================================================================
//====== Метод возвращает список идетнификаторов событий в соответствии с фильтром ===
//====================================================================================
QList<int> EventsMapModel::getIdEventsByFilter()
{
	QString queryStr;
	QList<int> eventIdList;

	queryStr.append("SELECT e.id_event \
					 FROM events e, event_objects e_o, type_event_object t_e, type_event tt	\
					 WHERE e.id_event = e_o.id_event \
					 AND e.id_type_event = tt.id_type_event \
					 AND e_o.id_type_event_object=t_e.id_type_event_object ");

	if(startPeriod->isValid() && endPeriod->isValid())
	{
		QDateTime startDateTime(*startPeriod,QTime(0,0,0));
		QDateTime endDateTime(*endPeriod,QTime(23,59,59));
	
		QString startDateT = startDateTime.toString("yyyy-MM-dd hh:mm:ss");
		QString endDateT = endDateTime.toString("yyyy-MM-dd hh:mm:ss");
		queryStr.append(QString("AND (e.time_event_start <= '%2' AND e.time_event_end >= '%1') ").arg(startDateT).arg(endDateT));
	}
	else
	{
		return eventIdList;
	}
	//------- статусы событий --------
	int checkedStatusCount = checkedItemsCount(*statesModel);
	if(checkedStatusCount > 0)
	{
		QString orStr;
		bool firstOrFlag = true;
		for(int row=0;row<statesModel->rowCount();row++)
		{
			if(statesModel->item(row,0)->checkState() == Qt::Checked)
			{
				int idStatus = statesModel->data(statesModel->index(row,0),Qt::UserRole).toInt();
				if(firstOrFlag)
				{
					orStr.append(QString("e.id_event_status = %1 ").arg(idStatus));
					firstOrFlag = false;
				}
				else
				{
					orStr.append(QString("OR e.id_event_status = %1 ").arg(idStatus));
				}
			}
		}
		queryStr.append(QString("AND (%1) ").arg(orStr));
	}
	//------- типы событий --------
	int checkedTypesCount = checkedItemsCount(*eventTypesModel);
	if(checkedTypesCount > 0)
	{
		QString orStr;
		bool firstOrFlag = true;
		for(int row=0;row<eventTypesModel->rowCount();row++)
		{
			if(eventTypesModel->item(row,0)->checkState() == Qt::Checked)
			{
				int idType = eventTypesModel->data(eventTypesModel->index(row,0),Qt::UserRole).toInt();
				if(firstOrFlag)
				{
					orStr.append(QString("e.id_type_event = %1 ").arg(idType));
					firstOrFlag = false;
				}
				else
				{
					orStr.append(QString("OR e.id_type_event = %1 ").arg(idType));
				}
			}
		}
		queryStr.append(QString("AND (%1) ").arg(orStr));
	}
	//------- объекты, с которыми связаны события --------
	if(objectsModel->rowCount() > 0)
	{
		QString orStr;
		bool firstOrFlag = true;
		for(int row=0;row<objectsModel->rowCount();row++)
		{
			int idObject = objectsModel->data(objectsModel->index(row,0),Qt::UserRole).toInt();
			if(firstOrFlag)
			{
				orStr.append(QString("e_o.id_object = %1 ").arg(idObject));
				firstOrFlag = false;
			}
			else
			{
				orStr.append(QString("OR e_o.id_object = %1 ").arg(idObject));
			}
		}
		queryStr.append(QString("AND (%1) ").arg(orStr));
	}
	queryStr.append(QString("ORDER BY e.id_event"));
	//---------------------------------------------------------------------------
	
	QSqlQuery query;
	if(query.exec(queryStr))
	{
		while(query.next())
		{		
			eventIdList.append(query.value(0).toInt());	
		}
	}
	return eventIdList;
}




//==========================================================================
//====== Метод возвращает число выбранных элементов модели =================
//==========================================================================
int EventsMapModel::checkedItemsCount(const QStandardItemModel &model)
{
	int count=0;

	for(int row=0;row<model.rowCount();row++)
	{
		if(model.item(row)->checkState() == Qt::Checked)
		{
			count++;
		}
	}

	return count;
}