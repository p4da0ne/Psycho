#include "signdata.h"


SignData::SignData(QString code, QList<Coord*> coordList, QMap<long int, QString> semList)
{
	this->signCode = code;
	this->metric = coordList;
	this->semantic = semList;
}


SignData::~SignData()
{
	
}



QString SignData::getSignCode()
{
	return signCode;
}


QList<Coord*> SignData::getMetricList()
{
	return metric;
}


QMap<long int, QString> SignData::getSemanticList()
{
	return semantic;
}