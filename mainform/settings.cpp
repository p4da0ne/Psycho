#include "settings.h"

Settings::Settings(QObject *parent)
	: QObject(parent)
{
	settings_db_connect= new QSettings("vka_db","saturn");
	id_db_server_ = settings_db_connect->value("/settings_db_connect/id_db_server","1").toInt();
	_db_name=settings_db_connect->value("/settings_db_connect/db_name","SATURN").toString();
	_user_name=settings_db_connect->value("/settings_db_connect/user_name","postgres").toString();
	_user_password=settings_db_connect->value("/settings_db_connect/user_password","").toString();
	_host=settings_db_connect->value("/settings_db_connect/host","localhost").toString();
}

Settings::~Settings()
{

}

void Settings::get_db_settings()
{

}
int Settings::id_db_server(){
	return id_db_server_;
}

QString Settings::db_name(){
	return _db_name;
}

QString Settings::user_name(){
	return _user_name;
}

QString Settings::user_password(){
	return _user_password;
}

QString Settings::host(){
	return _host;
}

void Settings::set_id_db_server(int set_id_db_server){
	id_db_server_=set_id_db_server;
	settings_db_connect->setValue("/settings_db_connect/id_db_server",id_db_server_);
}

void Settings::set_db_name(QString set_db_name){
	_db_name=set_db_name;
	settings_db_connect->setValue("/settings_db_connect/db_name",_db_name);
}

void Settings::set_user_name(QString set_user_name){
	_user_name=set_user_name;
	settings_db_connect->setValue("/settings_db_connect/user_name",_user_name);
}

void Settings::set_user_password(QString set_user_password){
	_user_password=set_user_password;
	settings_db_connect->setValue("/settings_db_connect/user_password",_user_password);
}

void Settings::set_host(QString set_host){
	_host=set_host;
	settings_db_connect->setValue("/settings_db_connect/host",_host);
}