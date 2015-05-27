#ifndef EVENTS_ADAPTER_H
#define EVENTS_ADAPTER_H

#include <QObject>
#include <QStandardItemModel>
#include "signdata.h"
//#include "coord.h"
#include "mapscroll.h"
#include <saturnSignTypes.h>


class EventsAdapter : public QObject
{
 public:
    EventsAdapter(QDate *startPeriod=0,QDate *endPeriod=0,
				  QStandardItemModel *objectsModel=0,QStandardItemModel *statesModel=0,
				  QStandardItemModel *eventTypesModel=0,QObject *parent=0);
    ~EventsAdapter();

	
public:
	Coord * WGStoPlane(long int hMap,Coord *coordObject);
	Coord * planeToWGS(long int hMap,Coord *coordObject);
	
	QList<SignData*> getEvents(long int hMap,double x1,double y1,double x2,double y2);
private:
	QDate *startPeriod;
	QDate *endPeriod;
	QStandardItemModel *objectsModel;
	QStandardItemModel *statesModel;
	QStandardItemModel *eventTypesModel;
};

#endif // EVENTS_ADAPTER_H
