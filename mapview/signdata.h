#ifndef SIGNDATA_H
#define SIGNDATA_H

#include "coord.h"

#include <QString>
#include <QList>
#include <QMap>

class SignData 
{
 private:
	QString signCode;
	QList<Coord*> metric;
	QMap<long int, QString> semantic;
 public:
	SignData(QString code, QList<Coord*> coordList, QMap<long int, QString> semList);
	~SignData();


	QString getSignCode();
	QList<Coord*> getMetricList();
	QMap<long int, QString> getSemanticList();
};

#endif // SIGNDATA_H
