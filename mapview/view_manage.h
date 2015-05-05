#ifndef VIEW_MANAGE_H
#define VIEW_MANAGE_H

#include <QObject>
#include "signdata.h"
#include "coord.h"


class ViewManage : public QObject
{
 public:
    /// Default constructor
    explicit ViewManage(QObject *parent=0);
    /// Destructor
    ~ViewManage();

	
public:
	QList<SignData*> getSmiMeans();
	QList<SignData*> getFormationsMeans();
	QList<SignData*> getGroupsMeans();
	QList<SignData*> getFormations();
	QList<SignData*> getSpecialConditions();

	enum SignObjectsTypes {FORMATIONS = 1, SPECIAL_CONDITIONS = 2, SMI_MEANS = 3, FORMATIONS_MEANS = 4, GROUPS_MEANS = 5};
};

#endif // VIEW_MANAGE_H
