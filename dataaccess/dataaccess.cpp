#include "dataaccess.h"

DataAccess::DataAccess(QObject *parent)
	: QObject(parent){

}

DataAccess::~DataAccess(){

}

bool DataAccess::create_connection(QString db_driver_,QString database_,QString user_,QString user_password_,QString hostname_,int port_){
	db_driver=db_driver_;
	database=database_;
	user=user_;
	user_password=user_password_;
	hostname=hostname_;
	port=port_;
	db=QSqlDatabase::addDatabase(db_driver);
	db.setDatabaseName(database);
	db.setUserName(user);
	db.setPassword(user_password);
	db.setHostName(hostname);
	db.setPort(port);
	if(!db.open()){
		message = db.lastError().text();
		connection_flag = false;
		return false;
	}
	else{
		message = ("Соединение с базой данных установлено");
		connection_flag = true;
		return true;
	}
}

bool DataAccess::get_connection_flag(){
	return connection_flag;
}

QString DataAccess::get_message(){
	return message;
}

bool DataAccess::close_connection_(){
    db.close();
	if(db.isOpen() == false){
		connection_flag = false;
		return true;
	}
	else {
		return false;
	}
}

QString DataAccess::get_user(){
	return user;
}

QString DataAccess::get_user_password(){
	return user_password;
}

QString DataAccess::get_hostname(){
	return hostname;
}

void DataAccess::set_user(QString new_user){
	user=new_user;
}

void DataAccess::set_user_password(QString new_user_password){
	user_password=new_user_password;
}

void DataAccess::set_hostname(QString new_hostname){
	hostname=new_hostname;
}