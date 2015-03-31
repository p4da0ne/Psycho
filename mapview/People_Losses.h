#ifndef PEOPLE_LOSSES_H
#define PEOPLE_LOSSES_H

#include <QString>
#include <QSqlQuery>
#include <QVariant>

class People_Losses
{
public:
	People_Losses();
	~People_Losses();
	void get_losses(int id_ls);
	void get_losses(QString id_ls);
	int min[3][4];
	int max[3][4];
	
};

#endif // PEOPLE_LOSSES_H
