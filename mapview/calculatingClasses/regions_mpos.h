/*!
************************************************************************************
** Класс расчетных задач по оценке морально-психологического состояния в регионах **
************************************************************************************
*/

#ifndef REGIONS_MPOS_H
#define REGIONS_MPOS_H

#include <QObject>
#include "signdata.h"
#include "coord.h"
#include "mapscroll.h"
#include "Calculate_K_omkrf.h"

class RegionsMpos : public QObject
{
 public:
    /// Default constructor
    explicit RegionsMpos(QObject *parent=0);
    /// Destructor
    ~RegionsMpos();

	
public:
	Coord * WGStoPlane(long int hMap,Coord *coordObject);

	QList<SignData*> getRegions(long int hMap,double x1,double y1,double x2,double y2);
	bool isRegionOnMap(long int hMap,int idRegion,double x1,double y1,double x2,double y2);
	QString getRegionExCode(int idRegion);
	QList<Coord*> getRegionMetric(long int hMap,int idRegion);
	QMap<long int,QString> getRegionSemantics(int idRegion);
	QString getRegionMpos(int idRegion);
	Calculate_K_omkrf *regionCalculator;
	

};

#endif // REGIONS_MPOS_H
