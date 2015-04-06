#ifndef DB_SATURN_H
#define DB_SATURN_H

#include "config_db_saturn.h"
#include <QObject>
#include "QMap"
#include "QStringList"
#include <dataaccess.h>
#include "db_struct.h"


class __EXPORT_DB_SATURN db_saturn : public DataAccess
{
	Q_OBJECT
public:
    db_saturn(DataAccess *parent=0);
    ~db_saturn();
	QMap<int, QString> get_all_military_rank();

private:
	  db_struct *db_st;

    
};

#endif // DB_SATURN_H
