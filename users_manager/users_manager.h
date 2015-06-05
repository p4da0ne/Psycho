#ifndef USERS_MANAGER_H
#define USERS_MANAGER_H

#include <QDialog>
#include "config_users_manager.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QToolButton>




class __EXPORT_USERS_MANAGER UsersManager: public QDialog
{
    Q_OBJECT
	
public:
    UsersManager(QDialog *parent = 0);
    virtual ~UsersManager();
	
private:
	QStandardItemModel *usersModel;
	QTableView *usersView;
	QToolButton *addUserButton;
	QToolButton *editUserButton;
	QToolButton *delUserButton;

	void fillUsersModel();
};


#endif  //USERS_MANAGER_H
