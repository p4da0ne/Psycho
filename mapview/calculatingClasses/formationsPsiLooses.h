/*!
************************************************************************************
** Класс расчетных задач по оценке психогенных потерь формирований *****************
************************************************************************************
*/

#ifndef PSI_LOOSES_H
#define PSI_LOOSES_H

#include <QObject>
#include "signdata.h"
#include "coord.h"
#include "mapscroll.h"
#include "People_Losses.h"

class FormationsPsiLooses : public QObject
{
 public:
    /// Default constructor
    explicit FormationsPsiLooses(QObject *parent=0);
    /// Destructor
    ~FormationsPsiLooses();

	
public:
	Coord * WGStoPlane(long int hMap,Coord *coordObject);
	QString getPsiLosses(int);
	QList<SignData*> getPsiFormationsLooses(long int hMap,double x1,double y1,double x2,double y2);

private:
	People_Losses *psiCalculator;

};

#endif // PSI_LOOSES_H
