#ifndef VIEW_MANAGE_H
#define VIEW_MANAGE_H

#include <QObject>
#include "signdata.h"
#include "coord.h"
#include "mapscroll.h"


class ViewManage : public QObject
{
 public:
    /// Default constructor
    explicit ViewManage(QObject *parent=0);
    /// Destructor
    ~ViewManage();

	
public:
	Coord * WGStoPlane(long int hMap,Coord *coordObject);

	QList<SignData*> getSmiMeans(long int hMap,double x1,double y1,double x2,double y2);
	QList<SignData*> getFormationsMeans(long int hMap,double x1,double y1,double x2,double y2);
	QList<SignData*> getGroupsMeans(long int hMap,double x1,double y1,double x2,double y2);
	QList<SignData*> getFormations(long int hMap,double x1,double y1,double x2,double y2);
	QList<SignData*> getSpecialConditions(long int hMap,double x1,double y1,double x2,double y2);

	enum SignObjectsTypes {FORMATIONS = 1, SPECIAL_CONDITIONS = 2, SMI_MEANS = 3, FORMATIONS_MEANS = 4, GROUPS_MEANS = 5};
};

#endif // VIEW_MANAGE_H
