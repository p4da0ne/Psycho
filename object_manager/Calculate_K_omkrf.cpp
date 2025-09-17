#include "Calculate_K_omkrf.h"


Calculate_K_omkrf::Calculate_K_omkrf(void)
{
    QTextCodec::codecForName("windows-1251");
}


Calculate_K_omkrf::~Calculate_K_omkrf(void)
{
}

int Calculate_K_omkrf::calc_K_snf()
{
	float rez=0;
	for(int i=0;i<17;i++)
		rez+=y_snf.at(i)*p_snf.at(i);
	rez /= 17;
	K_snf = rez;
	return 0;	
}

int Calculate_K_omkrf::calc_K_kf()
{
	float rez=0;
	for(int i=0;i<10;i++)
		rez+=y_kf.at(i)*p_kf.at(i);
	rez /= 10;
	K_kf = rez;
	return 0;
}

int Calculate_K_omkrf::calc_K_rf()
{
	float rez=0;
	for(int i=0;i<6;i++)
		rez+=y_rf.at(i)*p_rf.at(i);
	rez /= 6;
	K_rf = rez;
	return 0;
}

int Calculate_K_omkrf::calc_Rezultat()
{
	calc_K_snf();
	calc_K_kf();
	calc_K_rf();
	rez = K_snf * P_snf + K_kf * P_kf + K_rf * P_rf;
	rez /= 3;
	return 0;
}

int Calculate_K_omkrf::set_parametr(QString id_region)
{
	if(id_region.isEmpty())
		return -1;
	float x,y;
	y_kf.clear();
	p_kf.clear();
	y_snf.clear();
	p_snf.clear();
	y_rf.clear();
	p_rf.clear();
	QSqlQuery query;
	QString query_str = QString("select * from region where id_region = %1;").arg(id_region);
	query.exec(query_str);
    if(query.next()){
		for(int i=0;i<17;i++){
			x = query.value(13+2*i).toString().toFloat();
			y = query.value(13+2*i+1).toString().toFloat();
			y_snf << x;
			p_snf << y;
		}
		for(int i=0;i<10;i++){
			x = query.value(47+2*i).toString().toFloat();
			y = query.value(47+2*i+1).toString().toFloat();
			y_kf << x;
			p_kf << y;
		}
		for(int i=0;i<6;i++){
			x = query.value(67+2*i).toString().toFloat();
			y = query.value(67+2*i+1).toString().toFloat();
			y_rf << x;
			p_rf << y;
		}
		P_snf = query.value(79).toString().toFloat();
		P_kf = query.value(80).toString().toFloat();
		P_rf = query.value(81).toString().toFloat();
		return 0;
    }
	return -1;
}

float Calculate_K_omkrf::get_Rez_on_id_region(int id_region)
{
	set_parametr(QString::number(id_region));
	calc_Rezultat();
	return rez;
}

float Calculate_K_omkrf::get_Rez_on_id_region(QString id_region)
{
	set_parametr(id_region);
	calc_Rezultat();
	return rez;
}
