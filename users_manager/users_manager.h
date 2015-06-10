#ifndef USERS_MANAGER_H
#define USERS_MANAGER_H

#include <QDialog>
#include "config_users_manager.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QToolButton>
#include <QMessageBox>
#include "user_data_dialog.h"
#include "password_dialog.h"


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
	QToolButton *passwordButton;
	UserDataDialog *userDlg;
	PasswordDialog *passwordDlg;

	void fillUsersModel();
	int selectedUsersCount();
	void showMessageToUser(const QString message);
	void deleteSelectedUsers();
	void addUserInDB();
	void updateUserDataInDB();
	void updateUserPasswordInDB(int idUser,QString newPassword);
	QString str_to_md5(QString str);

private slots:
	void addUser();
	void editUser();
	void deleteUser();
	void changeUserPassword();
};


#endif  //USERS_MANAGER_H
