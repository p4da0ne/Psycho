#ifndef MOVE_MODEL_H
#define MOVE_MODEL_H

#include <QObject>
#include "signdata.h"
#include "../coord/coord.h"
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
	bool updateEventsCoordinates(int idSpecCond, Coord *coord);
	


	bool updateCoordinates(int idCoordinates, Coord *coord);

};

#endif // MOVE_MODEL_H
