#include "People_Losses.h"

People_Losses::People_Losses()
{

}

People_Losses::~People_Losses()
{

}

void People_Losses::get_losses(QString id_ls)
{
	QString str = QString("SELECT counte_ls FROM ls WHERE id_ls = %1").arg(id_ls);
	QSqlQuery query;
	query.exec(str);
	if(query.next())
	{
		int people_count = query.value(0).toInt();
		min[0][0] = (int)((double)people_count * 0.05 * 0.8);
		max[0][0] = (int)((double)people_count * 0.05 * 0.8);
		min[0][1] = (int)((double)people_count * 0.05 * 0.2 * 0.65);
		max[0][1] = (int)((double)people_count * 0.05 * 0.2 * 0.65);
		min[0][2] = (int)((double)people_count * 0.05 * 0.2 * 0.1);
		max[0][2] = (int)((double)people_count * 0.05 * 0.2 * 0.1);
		min[0][3] = (int)((double)people_count * 0.05 * 0.2 * 0.25);
		max[0][3] = (int)((double)people_count * 0.05 * 0.2 * 0.25);

		min[1][0] = (int)((double)people_count * 0.1 * 0.8);
		max[1][0] = (int)((double)people_count * 0.15 * 0.8);
		min[1][1] = (int)((double)people_count * 0.1 * 0.2 * 0.65);
		max[1][1] = (int)((double)people_count * 0.15 * 0.2 * 0.65);
		min[1][2] = (int)((double)people_count * 0.1 * 0.2 * 0.1);
		max[1][2] = (int)((double)people_count * 0.15 * 0.2 * 0.1);
		min[1][3] = (int)((double)people_count * 0.1 * 0.2 * 0.25);
		max[1][3] = (int)((double)people_count * 0.15 * 0.2 * 0.25);

		min[2][0] = (int)((double)people_count * 0.2 * 0.8);
		max[2][0] = (int)((double)people_count * 0.3 * 0.8);
		min[2][1] = (int)((double)people_count * 0.2 * 0.2 * 0.65);
		max[2][1] = (int)((double)people_count * 0.3 * 0.2 * 0.65);
		min[2][2] = (int)((double)people_count * 0.2 * 0.2 * 0.1);
		max[2][2] = (int)((double)people_count * 0.3 * 0.2 * 0.1);
		min[2][3] = (int)((double)people_count * 0.2 * 0.2 * 0.25);
		max[2][3] = (int)((double)people_count * 0.3 * 0.2 * 0.25);
		return;
	}
	else
	{
		return;
	}
}

void People_Losses::get_losses(int id_ls)
{
	QString str = QString("SELECT counte_ls FROM ls WHERE id_ls = %1").arg(id_ls);
	QSqlQuery query;
	query.exec(str);
	if(query.next())
	{
		int people_count = query.value(0).toInt();
		min[0][0] = (int)((double)people_count * 0.05 * 0.8);
		max[0][0] = (int)((double)people_count * 0.05 * 0.8);
		min[0][1] = (int)((double)people_count * 0.05 * 0.2 * 0.65);
		max[0][1] = (int)((double)people_count * 0.05 * 0.2 * 0.65);
		min[0][2] = (int)((double)people_count * 0.05 * 0.2 * 0.1);
		max[0][2] = (int)((double)people_count * 0.05 * 0.2 * 0.1);
		min[0][3] = (int)((double)people_count * 0.05 * 0.2 * 0.25);
		max[0][3] = (int)((double)people_count * 0.05 * 0.2 * 0.25);

		min[1][0] = (int)((double)people_count * 0.1 * 0.8);
		max[1][0] = (int)((double)people_count * 0.15 * 0.8);
		min[1][1] = (int)((double)people_count * 0.1 * 0.2 * 0.65);
		max[1][1] = (int)((double)people_count * 0.15 * 0.2 * 0.65);
		min[1][2] = (int)((double)people_count * 0.1 * 0.2 * 0.1);
		max[1][2] = (int)((double)people_count * 0.15 * 0.2 * 0.1);
		min[1][3] = (int)((double)people_count * 0.1 * 0.2 * 0.25);
		max[1][3] = (int)((double)people_count * 0.15 * 0.2 * 0.25);

		min[2][0] = (int)((double)people_count * 0.2 * 0.8);
		max[2][0] = (int)((double)people_count * 0.3 * 0.8);
		min[2][1] = (int)((double)people_count * 0.2 * 0.2 * 0.65);
		max[2][1] = (int)((double)people_count * 0.3 * 0.2 * 0.65);
		min[2][2] = (int)((double)people_count * 0.2 * 0.2 * 0.1);
		max[2][2] = (int)((double)people_count * 0.3 * 0.2 * 0.1);
		min[2][3] = (int)((double)people_count * 0.2 * 0.2 * 0.25);
		max[2][3] = (int)((double)people_count * 0.3 * 0.2 * 0.25);
		return;
	}
	else
	{
		return;
	}
}