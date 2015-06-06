#ifndef MAINFORM_H
#define MAINFORM_H

#include <QtGui>
#include <QMap>
#include <QStringList>
#include <QFile>
#include <QMdiSubWindow>

#include "mymdiarea.h"
#include "settings.h"
#include "mapsettingsdialog.h"
#include "dbbackup.h"

#include <manage_users.h>
#include <users_manager.h>
#include <output.h>
#include <reports.h>
#include <supporting_tables.h>
#include <db_saturn.h>
#include <dataaccess.h>




namespace Ui
{
    class main_form;
}

class Mainform : public QMainWindow
{
	Q_OBJECT

signals:
   void reopen_login();
   void valid_user(int id_user);

public:
	Mainform(QMainWindow *parent = 0, Qt::WFlags flags = 0);
	virtual ~Mainform();
	QString db_driver,message,login_message;
	int id_user;
	bool login_flag;
	QString str_to_md5(QString str);
	myQMdiArea * m_mdiArea;

	QMdiSubWindow * map_window;
	QMdiSubWindow * obmanager;
	QMdiSubWindow * signs_window;
	QMdiSubWindow * events_window;


    
private slots:

	void show_connect_settings_dialog();
	void test_connection();
	void save_connection_settings();
	void show_login_form();
	void create_user_menu(int id_user);
	void show_map_form();
	void show_supporting_tables_form();
	void show_object_manager_form();
    void show_signs_edit();
	void slotOpenMapSettingsDialog();
	void slotOpenBackupDbDialog();
	void slotOpenEventManagerForm();
	void closeTab(int i);

	////////////////////////////////////
	void slotOpenUserManageForm();
	///////////////////////////////////

private:
	QMenu * menu;
	QAction * action1;
	QAction * action2;
	QAction * action_adm;
	QMenu * settings_menu;
	QAction * sett_act1;
	QAction * sett_act2;
	QAction * sett_act3;
    QAction * sett_act5;
	QAction * event_act;

	QMenu * DB_work_menu;
	QMenu * oper_menu;
	QAction * map_act;
	QAction * open_map_sett_action;
	QAction * backup_db_action;
///////////////////////////////////////////////////
	QAction * users_action;
	/////////////////////////////

	QLabel *message_label;
	void init_menu(int id_user_group);
	QDialog *DB_dialog;
	QDialog *login_form;
	QLineEdit *db_name_edit;
	QLineEdit *host_edit;
	QLineEdit *user_edit;
	QLineEdit *password_edit;
	QLineEdit *login_edit;
	QLineEdit *login_password_edit;
	QComboBox *ServDB_combobox;
	Settings *settings;





    bool create_connection(QString,QString,QString,QString,QString);
	bool close_connection();
	void get_db_settings();
	bool test_db_connection();
	int login(QString login_name,QString password);
	void add_mapwork(QMenu *oper_menu);
	void add_menu_db_connection(QMenu *settings_menu);
	void add_menu_manage_users(QMenu *settings_menu);
	void add_menu_map_settings(QMenu *settings_menu);
	void add_menu_backup_db(QMenu *settings_menu);
    void add_menu_signs_edit(QMenu *settings_menu);
	void add_menu_exit(QMenu *menu);
	void add_menu_change_user(QMenu *menu);
	void add_menu_enter_system(QMenu *menu);
	void add_menu_supporting_tables(QMenu *oper_menu);
	void add_menu_object_manager(QMenu *oper_menu);
	void add_menu_event_manager(QMenu *oper_menu);
	bool connection_flag;
	db_saturn *db;

	Ui::main_form *UI;
};

#endif // MAINFORM_H
