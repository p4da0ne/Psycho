#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QObject>
#include <QString>
#include <QtSql>
#include "config_dataaccess.h"

class __EXPORT_DATAACCESS DataAccess : public QObject

{
	
public:
	DataAccess(QObject *parent);
	~DataAccess();
	bool create_connection(QString db_driver_, QString database_, QString user_, QString user_passwd_, QString hostname_, int port_=5432);
	bool get_connection_flag();
	QString get_message();
	bool close_connection_();
	QString get_user_password();
	QString get_hostname();
	QString get_user();
	void set_user(QString new_user);
	void set_user_password(QString new_user_password);
	void set_hostname(QString new_hostname);

protected:
	QSqlDatabase db;
	bool connection_flag;
	QString message;
	QString db_driver;
	QString database;
	QString user;
	QString	user_password;
	QString	hostname;
	int port;
};


#endif // DATAACCESS_H
