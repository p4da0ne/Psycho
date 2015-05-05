#ifndef CALCULATING_MPS_H
#define CALCULATING_MPS_H

#include <QSqlQuery>
#include <QString>
#include <QVariant> 

class calculating_mps 
{
public:
	calculating_mps();
	~calculating_mps();
	float get_mps_ls()
	{	return mps_ls;	}
	float calculating(QString id_ls);
	float calculating(int id_ls);
	QString isEnemy(QString id_ls);
	QString isEnemy(int id_ls);
	float get_mps_ofec()
	{
		return mps_ofec;
	}
	float get_mps_kont()
	{
		return mps_kont;
	}
	float get_mps_priz()
	{
		return mps_priz;
	}

private:
	float mps_ofec;
	float mps_kont;
	float mps_priz;
	float mps_ls;	
};

#endif // CALCULATING_MPS_H
