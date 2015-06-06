#ifndef USERS_MANAGER_H
#define USERS_MANAGER_H

#include <QDialog>
#include "config_users_manager.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QToolButton>
#include <QMessageBox>




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
	int selectedUsersCount();
	void showMessageToUser(const QString message);
	void deleteSelectedUsers();


private slots:
	void addUser();
	void editUser();
	void deleteUser();
};


#endif  //USERS_MANAGER_H
