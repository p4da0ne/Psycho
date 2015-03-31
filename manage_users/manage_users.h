#ifndef MANAGE_USERS_H
#define MANAGE_USERS_H

#include <QDialog>
#include "config_manage_users.h"
#include "ui_manage_users_form.h"
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

namespace Ui
{
    class manage_users_form;
};


class __EXPORT_MANAGE_USERS ManageUsers: public QDialog
{
    Q_OBJECT
protected:
	QTableWidgetItem *Item;
	QDialog *a_u;
	QDialog *ui_del_user;
	
public:
    ManageUsers(QDialog *parent = 0);
    virtual ~ManageUsers();
	
	

	void clear_tableWidget(QTableWidget *);
	int user_id_max;
	QLabel *Fmaly;
	QLabel *Name;
	QLabel *Otchestvo;
	QLabel *login;
	QLabel *password;
	QLabel *group;
	QLabel *military_rank;
	QComboBox *Group;
	QComboBox *military_rang;
	QLineEdit *Fmaly_data;
	QLineEdit *Name_data;
	QLineEdit *Otchestvo_data;
	QLineEdit *login_data;
	QLineEdit *password_data;
	QPushButton *ok;
	QPushButton *cansel;
	QLabel *error;
	QLabel *stac;
	QString str_to_md5(QString str);
	QLabel *del_user_name;
	QLineEdit *id_del_user;
	QPushButton *del_user_pb;
	QCheckBox *cbx;
	QDialog *pas;
	QLineEdit *pas_data;
public slots:
	void view_users();
	void add_user_ok();
	void add_user_exit();
	void add_user();
	void del_user();
	void del_user_run(int id_del_user);
	void errors_message(QString str);
	void save();
	void ReWrite_password();

private:
    Ui::manage_users_form *UI;

	
};


#endif 