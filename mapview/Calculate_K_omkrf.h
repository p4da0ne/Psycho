#ifndef CALCULATE_K_OMKRF
#define CALCULATE_K_OMKRF

#include <QString.h>
#include <QList.h>
#include <QTextcodec.h>
#include <QSqlquery.h>
#include <QSql.h>
#include <QVariant.h>
#include <QChar.h>
#include <qstandarditemmodel.h>

class Calculate_K_omkrf
{
public:
	Calculate_K_omkrf(void);
	~Calculate_K_omkrf(void);
	int calc_Rezultat();
	float getRezultate(){
		return rez;
	}
	QString getStrRezultate(){
		return rez_Str;
	}

	float get_Rez_on_id_region(int id_region);
	float get_Rez_on_id_region(QString id_region);
	int set_parametr(QString id_region);

private:
	float K_snf;
	float K_kf;
	float K_rf;
	float P_snf;
	float P_kf;
	float P_rf;
	float rez;
	QString rez_Str;
	QList<float> y_snf;
	QList<float> y_kf;
	QList<float> y_rf;
	QList<float> p_snf;
	QList<float> p_kf;
	QList<float> p_rf;

	int calc_K_snf();
	int calc_K_kf();
	int calc_K_rf();

};

#endif // CALCULATE_K_OMKRF
