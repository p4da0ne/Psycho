#ifndef EVENTS_MAP_H
#define EVENTS_MAP_H

#include <QObject>
#include <QStandardItemModel>
#include "signdata.h"
#include "mapscroll.h"
#include <saturnSignTypes.h>


class EventsMapModel : public QObject
{
 public:
    EventsMapModel(QDate *startPeriod,QDate *endPeriod,
				  QStandardItemModel *objectsModel=0,QStandardItemModel *statesModel=0,
				  QStandardItemModel *eventTypesModel=0,QObject *parent=0);

	EventsMapModel(QObject *parent=0);
    ~EventsMapModel();

	
public:
	Coord * WGStoPlane(long int hMap,Coord *coordObject);
	Coord * planeToWGS(long int hMap,Coord *coordObject);
	QStandardItemModel * getEventStatusList();
	QStandardItemModel * getEventTypesList();


	QList<SignData*> getEvents(long int hMap,double x1,double y1,double x2,double y2);

	QString createEventsFilterQuery();
	bool isEventOnMap(const int idEvent,long int hMap,double x1,double y1,double x2,double y2);

	QList<Coord*> getEventCoordinates(long int hMap,const int idEvent);
	QList<Coord*> getObjectCoordinates(long int hMap,int idObject, QString tableName);
	QString getEventSignCode(const int idTypeEvent);
	int checkedItemsCount(const QStandardItemModel &model);
	int getEventStatusId(int idEvent);

private:
	QDate *startPeriod;
	QDate *endPeriod;
	QStandardItemModel *objectsModel;
	QStandardItemModel *statesModel;
	QStandardItemModel *eventTypesModel;
};

#endif // EVENTS_MAP_H
