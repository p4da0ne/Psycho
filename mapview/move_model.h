#ifndef MOVE_MODEL_H
#define MOVE_MODEL_H

#include <QObject>
#include "signdata.h"
#include "coord.h"
#include "mapscroll.h"
#include <saturnSignTypes.h>


class MoveModel : public QObject
{
 public:
    /// Default constructor
    explicit MoveModel(QObject *parent=0);
    /// Destructor
    ~MoveModel();

	
public:
	Coord * WGStoPlane(long int hMap,Coord *coordObject);
	Coord * planeToWGS(long int hMap,Coord *coordObject);
	QList<Coord *> getObjectCoordinates(long int hMap,int idObject, int objectType);

	void updateObjectCoordinates(int idObject,int objectType,Coord *coord);

	bool updateFormationCoordinates(int idLs, Coord *coord);
	bool updatePersonesCoordinates(int idPersone, Coord *coord);
	bool updateMeansCoordinates(int idMeans, Coord *coord);
	bool updateSpecialConditionsCoordinates(int idSpecCond, Coord *coord);


	bool updateCoordinates(int idCoordinates, Coord *coord);

	//QList<SignData*> getSmiMeans(long int hMap,double x1,double y1,double x2,double y2);
	//QList<SignData*> getFormationsMeans(long int hMap,double x1,double y1,double x2,double y2);
	//QList<SignData*> getGroupsMeans(long int hMap,double x1,double y1,double x2,double y2);
	//QList<SignData*> getFormations(long int hMap,double x1,double y1,double x2,double y2);
	//QList<SignData*> getSpecialConditions(long int hMap,double x1,double y1,double x2,double y2);
	//QList<SignData*> getPersones(long int hMap,double x1,double y1,double x2,double y2);

	//QString getObjectTypeAndName(int idObject, int objectType);
	//QString getObjectInfo(int idObject, int objectType);
	//QString get_ls_info(int);
	//QString get_spec_cond_info(int);
	//QString get_means_info(int);
	//QString get_info_region(int);
	//QString get_object_foto_from_DB(int);
	//QString get_blok_foto_from_DB(int);

};

#endif // MOVE_MODEL_H
