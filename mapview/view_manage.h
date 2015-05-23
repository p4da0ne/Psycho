#ifndef VIEW_MANAGE_H
#define VIEW_MANAGE_H

#include <QObject>
#include "signdata.h"
#include "coord.h"
#include "mapscroll.h"
#include <saturnSignTypes.h>


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
	QList<SignData*> getPersones(long int hMap,double x1,double y1,double x2,double y2);
	QList<Coord*> getSCMetric(long int hMap,int idSC);
	bool isScOnMap(long int hMap,int idRegion,double x1,double y1,double x2,double y2);


	QString getObjectTypeAndName(int idObject, int objectType);
	QString getObjectInfo(int idObject, int objectType);
	QString get_ls_info(int);
	QString get_spec_cond_info(int);
	QString get_means_info(int);
	QString get_info_region(int);
	QString get_object_foto_from_DB(int);
	QString get_blok_foto_from_DB(int);
	QString get_mpo_foto_from_DB(int);
	QString get_srec_cond_foto_from_DB(int);
	QString get_info_personel(int);
	QString get_pers_foto_from_DB(int);

};

#endif // VIEW_MANAGE_H
