#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>

class Settings : public QObject
{
	Q_OBJECT

public:
	Settings(QObject *parent);
	~Settings();
	void get_db_settings();
	int id_db_server();
	QString db_name();
	QString user_name();
	QString user_password();
	QString host();
	void set_id_db_server(int );
	void set_db_name(QString );
	void set_user_name(QString);
	void set_user_password(QString);
	void set_host(QString);

private:
	QSettings *settings_db_connect;
	int id_db_server_;
	QString _db_name;
	QString _user_name;
	QString _user_password;
	QString _host;
};

#endif // SETTINGS_H
