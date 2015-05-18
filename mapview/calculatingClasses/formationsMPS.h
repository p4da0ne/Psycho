/*!
*****************************************************************************************************
** Класс расчетных задач по оценке морально-психологического состояний формирований *****************
*****************************************************************************************************
*/

#ifndef FORMATIONS_MPS_H
#define FORMATIONS_MPS_H

#include <QObject>
#include "signdata.h"
#include "coord.h"
#include "mapscroll.h"
#include "calculating_mps.h"

class FormationsMPS : public QObject
{
 public:
    /// Default constructor
    explicit FormationsMPS(QObject *parent=0);
    /// Destructor
    ~FormationsMPS();

	
public:
	Coord * WGStoPlane(long int hMap,Coord *coordObject);
	//QString getPsiLosses(int);
	QList<SignData*> getFormationsMPS(long int hMap,double x1,double y1,double x2,double y2,bool enemyFlag);

private:
	calculating_mps *mpsCalculator;

};

#endif // FORMATIONS_MPS_H
