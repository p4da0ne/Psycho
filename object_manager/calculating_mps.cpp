#include "calculating_mps.h"

calculating_mps::calculating_mps()
{
	mps_priz = -1;
	mps_kont = -1;
	mps_ofec = -1;
}

calculating_mps::~calculating_mps()
{

}

float calculating_mps::calculating(QString id_ls)
{
	QSqlQuery query;
	QString str = QString("SELECT mps_priz_ls, mps_konrt_ls, mps_of_ls FROM ls WHERE id_ls = %1").arg(id_ls);
	query.exec(str);
	if(query.next())
	{
		mps_priz = query.value(0).toString().toFloat();
		mps_kont = query.value(1).toString().toFloat();
		mps_ofec = query.value(2).toString().toFloat();
	}
	else
	{
		return -1;
	}
	mps_ls = (mps_kont + mps_priz + mps_ofec)/3;
	return mps_ls;
}

float calculating_mps::calculating(int id_ls)
{
	QSqlQuery query;
	QString str = QString("SELECT mps_priz_ls, mps_konrt_ls, mps_of_ls FROM ls WHERE id_ls = %1").arg(id_ls);
	query.exec(str);
	if(query.next())
	{
		mps_priz = query.value(0).toString().toFloat();
		mps_kont = query.value(1).toString().toFloat();
		mps_ofec = query.value(2).toString().toFloat();
	}
	else
	{
		return -1;
	}
	mps_ls = (mps_kont + mps_priz + mps_ofec)/3;
	return mps_ls;
}
QString calculating_mps::isEnemy(QString id_ls)
{
	QSqlQuery query;
	QString str = QString("SELECT enimy_ls FROM ls WHERE id_ls = %1").arg(id_ls);
	query.exec(str);
	if(query.next())
	{
		bool tmp = query.value(0).toBool();
		if(tmp)
			return QString("войск противника");
		else
			return QString("своих войск");
	}
	else
		return QString();

}
QString calculating_mps::isEnemy(int id_ls)
{
		QSqlQuery query;
	QString str = QString("SELECT enimy_ls FROM ls WHERE id_ls = %1").arg(id_ls);
	query.exec(str);
	if(query.next())
	{
		bool tmp = query.value(0).toBool();
		if(tmp)
			return QString("войск противника");
		else
			return QString("своих войск");
	}
	else
		return QString();
}