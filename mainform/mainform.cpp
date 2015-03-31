#include "mainform.h"

#if defined Q_OS_WIN
#define kodec QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
#else
#define kodec QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

Mainform::Mainform(QMainWindow *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
	UI (new Ui::main_form)
{
	UI->setupUi(this);
	settings=new Settings(this);   
	setWindowIcon(QIcon("./icons/metacontact.png"));
	connection_flag = false;
	login_flag = false;
	QImage img("./icons/saturn.png");
	m_mdiArea = new myQMdiArea(img,this);
	setCentralWidget(m_mdiArea);
	db=new db_saturn();
	connect(this,SIGNAL(reopen_login()),this,SLOT(show_login_form()));
	connect(this,SIGNAL(valid_user(int)),this,SLOT(create_user_menu(int)));
	connect(this,SIGNAL(signalMainWindowShown()),this,SLOT(show_login_form()));
	
	

//----------- Тестирование модуля вывода ---------
	QTextCodec *codec = QTextCodec::codecForName("CP1251");
	QTextCodec::setCodecForCStrings(codec);
	Reports *r = new Reports;
	QString report = r->create_object_formular(8);

//----------------------------------------
	init_menu(0);




////

//while (!test_db_connection())
//	{
//
//	}
//close_connection();
//
//
//	QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
//    db.setHostName("192.168.0.63");
//	db.setDatabaseName("SATURN_3");
//    db.setUserName("saturn_user");
//    db.setPassword("qwerty");
//	db.open();
//	
//
//
//
//init_menu(1);
//show_map_form();
//






}

Mainform::~Mainform()
{
 delete UI;
}



//========== Создание и открытие диалогового окна настроек соединения с БД ===============
void Mainform::show_connect_settings_dialog()
{
 DB_dialog = new QDialog;
 DB_dialog->setWindowTitle("Настройки соединения с БД");
 DB_dialog->setFixedSize(400,250);

 //============== Создание элементов формы ====================
 QLabel *ServDB_label = new QLabel("Сервер БД: "); 
 ServDB_combobox = new QComboBox;
 ServDB_combobox->addItem("PostgreSQL",1);
 ServDB_combobox->addItem("MySQL",2);
 ServDB_combobox->setCurrentIndex(settings->id_db_server()-1);
 
 QLabel *db_name_label = new QLabel("Имя БД: ");
 db_name_edit = new QLineEdit(settings->db_name());

 QLabel *host_label = new QLabel("Хост (ip-address): ");
 host_edit = new QLineEdit(settings->host()); 

 QLabel *user_label = new QLabel("Пользователь: ");
 user_edit = new QLineEdit(settings->user_name());

 QLabel *password_label = new QLabel("Пароль: ");
 password_edit = new QLineEdit();
 password_edit->setEchoMode(QLineEdit::Password);

 message_label = new QLabel;
 message_label->setWordWrap(true);

 QPushButton *test_button = new QPushButton("Тестирование соединения");
 QPushButton *ok_button = new QPushButton("Да");
 QPushButton *cancel_button = new QPushButton("Отмена");
 
 //=============== Размещение объектов на форме ===================
 QGridLayout *grid_layout = new QGridLayout;
 grid_layout->addWidget(ServDB_label,0,0);
 grid_layout->addWidget(ServDB_combobox,0,1);
 grid_layout->addWidget(db_name_label,1,0);
 grid_layout->addWidget(db_name_edit,1,1);
 grid_layout->addWidget(host_label,2,0);
 grid_layout->addWidget(host_edit,2,1);
 grid_layout->addWidget(user_label,3,0);
 grid_layout->addWidget(user_edit,3,1);
 grid_layout->addWidget(password_label,4,0);
 grid_layout->addWidget(password_edit,4,1);

 QBoxLayout *mess_layout = new QBoxLayout(QBoxLayout::LeftToRight);
 mess_layout->addWidget(message_label);

 QBoxLayout *buttons_layout = new QBoxLayout(QBoxLayout::LeftToRight);
 buttons_layout->addWidget(test_button);
 buttons_layout->addStretch();
 buttons_layout->addWidget(ok_button);
 buttons_layout->addWidget(cancel_button);

 QBoxLayout *main_layout = new QBoxLayout(QBoxLayout::TopToBottom);
 main_layout->addLayout(grid_layout);
 main_layout->addStretch();
 main_layout->addLayout(mess_layout);
 main_layout->addStretch();
 main_layout->addLayout(buttons_layout);

 DB_dialog->setLayout(main_layout);

 //=================================================================
 connect(test_button,SIGNAL(clicked()),this,SLOT(test_connection())); 
 connect(ok_button,SIGNAL(clicked()),this,SLOT(save_connection_settings()));  
 connect(cancel_button,SIGNAL(clicked()),DB_dialog,SLOT(accept())); 
 //=================================================================
 if(DB_dialog->exec() == QDialog::Accepted)
 {
	if (test_db_connection())
	{
		connection_flag = true;
	}
  	init_menu(0);
 }
 
 delete DB_dialog;
}


//==============================================================
void Mainform::get_db_settings()
{

}

//=============== Сохранение настроек соединения с БД =========================
void Mainform::save_connection_settings()
{
    int index = ServDB_combobox->currentIndex();
	QVariant id = ServDB_combobox->itemData(index);
	settings->set_id_db_server( id.toInt());
	settings->set_db_name( db_name_edit->text());	
	settings->set_user_name( user_edit->text());	
	settings->set_user_password( password_edit->text());
	settings->set_host( host_edit->text());	
	DB_dialog->accept();
	return;
}

//============= Проверка соединения с БД на форме настроек соединения =====================
void Mainform::test_connection()
{
	message_label->clear();
 
	int index = ServDB_combobox->currentIndex();
	QVariant id = ServDB_combobox->itemData(index);
	int id_drv = id.toInt();
	if (id_drv == 1) db_driver = "QPSQL";
	else if (id_drv == 2)  db_driver = "QMYSQL";
	else db_driver = "QPSQL";

	QString database = db_name_edit->text();	
	QString user = user_edit->text();	
	QString user_passwd = password_edit->text();	
	QString hostname = host_edit->text();	

 if (create_connection(db_driver,database,user,user_passwd,hostname))
 {
   message_label->setStyleSheet("QLabel { color : green; }");
   message_label->setText(db->get_message()); 
   close_connection();
 }
 else
 {
   message_label->setStyleSheet("QLabel { color : red; }");
   message_label->setText(db->get_message()); 
 }
 return;
}


//============= Проверка соединения с БД при первом входе ==================================
bool Mainform::test_db_connection()
{
	if (settings->id_db_server() == 1) db_driver = "QPSQL";
	else if (settings->id_db_server() == 2)  db_driver = "QMYSQL";
	else db_driver = "QPSQL";

	if (create_connection(db_driver,settings->db_name(),settings->user_name(),settings->user_password(),settings->host()))
	{
		return true; 
	}
	else
	{
		message = db->get_message();
		return false;
	}
}

//==================== Создание соединения с БД ====================================
bool Mainform::create_connection(QString db_driver, QString database, QString user, QString user_passwd, QString hostname)
{
	connection_flag = db->create_connection(db_driver, database, user, user_passwd, hostname);
	connection_flag = db->get_connection_flag();
	return connection_flag;
}



//=============== Закрытие соединения с БД =====================================
bool Mainform::close_connection()
{
	return db->close_connection_();
}


//=============== Инициализация меню главной формы ==========================
void Mainform::init_menu(int id_user_group)
{
	QString mess;
    switch(id_user_group)
	{
	 case 0:	   		//==== Незарегистрированный пользователь (вошел без логина и пароля)или нет соединения с БД ====
		    UI->menuBar->clear();
			menu = new QMenu("Файл");
            if(connection_flag == false)
			{
				mess = "Сатурн - нет соединения с БД";
			}
			else if(connection_flag == true)
			{
				add_menu_enter_system(menu);
				mess = "Сатурн - незарегистрированный пользователь";
			}
			add_menu_exit(menu);
			UI->menuBar->addMenu(menu);

			settings_menu = new QMenu("Настройки");
			add_menu_db_connection(settings_menu);			
			UI->menuBar->addMenu(settings_menu);
			
			Mainform::setWindowTitle(mess);

		 	break;

	 case 1:	   		//==== Разработчик (права админа) ====
			UI->menuBar->clear();
			menu = new QMenu("Файл");
			add_menu_change_user(menu);
			add_menu_exit(menu);
			UI->menuBar->addMenu(menu);
			
			settings_menu = new QMenu("Настройки");
			add_menu_db_connection(settings_menu);
			add_menu_manage_users(settings_menu);
			
			UI->menuBar->addMenu(settings_menu);

		//	DB_work_menu = new QMenu("Работа с БД");
		//	UI->menuBar->addMenu(DB_work_menu);

			oper_menu = new QMenu("Оперативная работа");
			add_menu_supporting_tables(oper_menu);
			add_menu_object_manager(oper_menu);
			add_menu_calculation(oper_menu);
			add_mapwork(oper_menu);
			UI->menuBar->addMenu(oper_menu);
			Mainform::setWindowTitle("Сатурн - сессия разработчика");
			break;
	 
	 case 2:     		//==== Администратор ====
			UI->menuBar->clear();
			menu = new QMenu("Файл");
			add_menu_change_user(menu);
			add_menu_exit(menu);		
			UI->menuBar->addMenu(menu);

			settings_menu = new QMenu("Настройки");
			add_menu_db_connection(settings_menu);
			add_menu_manage_users(settings_menu);
			
			UI->menuBar->addMenu(settings_menu);

		//	DB_work_menu = new QMenu("Работа с БД");
		//	UI->menuBar->addMenu(DB_work_menu);

			oper_menu = new QMenu("Оперативная работа");
			add_menu_supporting_tables(oper_menu);
			add_menu_object_manager(oper_menu);
			add_menu_calculation(oper_menu);
			add_mapwork(oper_menu);
			UI->menuBar->addMenu(oper_menu);

			Mainform::setWindowTitle("Сатурн - сессия администратора");
			break;

	 case 3://==== Пользователь ====
			UI->menuBar->clear();
			menu = new QMenu("Файл");
			add_menu_change_user(menu);
			add_menu_exit(menu);
			UI->menuBar->addMenu(menu);
			
	//		DB_work_menu = new QMenu("Работа с БД");
		//	UI->menuBar->addMenu(DB_work_menu);

			oper_menu = new QMenu("Оперативная работа");
		//	add_menu_supporting_tables(oper_menu);
			add_menu_object_manager(oper_menu);
			add_menu_calculation(oper_menu);
			add_mapwork(oper_menu);
			UI->menuBar->addMenu(oper_menu);

			Mainform::setWindowTitle("Сатурн - пользовательская сессия");
		 	break;
	}
return;
}

void Mainform::add_menu_enter_system(QMenu *menu){
	action1 = new QAction("Вход в систему",this);
	action1->setIcon(QIcon("./icons/enter.png"));
	menu->addAction(action1);
	connect(action1, SIGNAL(triggered()),this, SLOT(show_login_form()));
}

void Mainform::add_menu_change_user(QMenu *menu){
	action1 = new QAction("Сменить пользователя",this);
	action1->setIcon(QIcon("./icons/change_user.png"));
	menu->addAction(action1);
	connect(action1, SIGNAL(triggered()),this, SLOT(show_login_form()));
}

void Mainform::add_menu_exit(QMenu *menu){
	action2 = new QAction("Выход",this);
	action2->setIcon(QIcon("./icons/exit.png"));
	menu->addAction(action2);
	connect(action2, SIGNAL(triggered()),this, SLOT(close()));
}

void Mainform::add_mapwork(QMenu *oper_menu){
	map_act = new QAction("Работа с картой",this);
	map_act->setIcon(QIcon("./icons/mapwork.png"));
	oper_menu->addAction(map_act);
	connect(map_act, SIGNAL(triggered()),this, SLOT(show_map_form()));
}

void Mainform::add_menu_db_connection(QMenu *settings_menu){
	sett_act1 = new QAction("Подключение к БД",this);
	sett_act1->setIcon(QIcon("./icons/db_settings.png"));
	settings_menu->addAction(sett_act1);
	connect(sett_act1, SIGNAL(triggered()),this, SLOT(show_connect_settings_dialog()));
}

void Mainform::add_menu_manage_users(QMenu *settings_menu){
	sett_act4 = new QAction("Управление пользователями",this);
	sett_act4->setIcon(QIcon("./icons/user_config.png"));
	settings_menu->addAction(sett_act4);
	connect(sett_act4, SIGNAL(triggered()),this, SLOT(show_user_form()));
}

void Mainform::add_menu_supporting_tables(QMenu *oper_menu){
	sett_act2 = new QAction("Справочники",this);
	sett_act2->setIcon(QIcon("./icons/book.png"));
	oper_menu->addAction(sett_act2);
	connect(sett_act2, SIGNAL(triggered()),this, SLOT(show_supporting_tables_form()));
}

void Mainform::add_menu_object_manager(QMenu *oper_menu){
	sett_act3 = new QAction("Управление объектами",this);
	sett_act3->setIcon(QIcon("./icons/change_user.png"));
	oper_menu->addAction(sett_act3);
	connect(sett_act3, SIGNAL(triggered()),this, SLOT(show_object_manager_form()));
}
void Mainform::add_menu_calculation(QMenu *oper_menu){
	sett_act5 = new QAction("Расчетные задачи",this);
	sett_act5->setIcon(QIcon("./icons/text.png"));
	oper_menu->addAction(sett_act5);
	connect(sett_act5, SIGNAL(triggered()),this, SLOT(show_calculating_form()));
}
//============= Создание и открытие формы входа (смены) пользователя ==========
void Mainform::show_login_form()
{
 login_flag = false;
 login_form = new QDialog;
 login_form->setWindowTitle("Вход в систему");
 
 login_form->setFixedSize(300,200);
 
 //============== Создание элементов формы ====================
 QLabel *login_label = new QLabel("Имя пользователя: "); 
 login_edit = new QLineEdit;

 QLabel *password_label = new QLabel("Пароль: ");
 login_password_edit = new QLineEdit; 
 login_password_edit->setEchoMode(QLineEdit::Password);

 message_label = new QLabel(login_message);
 message_label->setWordWrap(true);

 QPushButton *ok_butt = new QPushButton("Да");
 QPushButton *cancel_butt = new QPushButton("Отмена");
 
 //=============== Размещение объектов на форме ===================
 QGridLayout *g_layout = new QGridLayout;
 g_layout->addWidget(login_label,0,0);
 g_layout->addWidget(login_edit,0,1);
 g_layout->addWidget(password_label,1,0);
 g_layout->addWidget(login_password_edit,1,1);

 QBoxLayout *m_layout = new QBoxLayout(QBoxLayout::LeftToRight);
 m_layout->addWidget(message_label);

 QBoxLayout *butt_layout = new QBoxLayout(QBoxLayout::LeftToRight);
 butt_layout->addStretch();
 butt_layout->addWidget(ok_butt);
 butt_layout->addWidget(cancel_butt);

 QBoxLayout *main_layout = new QBoxLayout(QBoxLayout::TopToBottom);
 main_layout->addLayout(g_layout);
 main_layout->addStretch();
 main_layout->addLayout(m_layout);
 main_layout->addStretch();
 main_layout->addLayout(butt_layout);

 login_form->setLayout(main_layout);

 //=================================================================

 connect(ok_butt,SIGNAL(clicked()),login_form,SLOT(accept()));  
 connect(cancel_butt,SIGNAL(clicked()),login_form,SLOT(close())); 
 //=================================================================
 if(login_form->exec() == QDialog::Accepted)
 {
	QString login_name =	login_edit->text();
	QString password =	login_password_edit->text();
	if(!login(login_name,password))
	{
		login_message = "<p align = 'center'><font color='red'>" + tr("Login or password are incorrect.") + "</font></p>";
		message_label->setText(login_message);
		delete login_form;	 
		reopen_login();
		return;
	}
	else
	{
		valid_user(id_user);  // сигнал входа легального пользователя
	}
 }
 delete login_form;
 login_message = "";
 return;
}


//============== Функция проверки логина и пароля пользователя в БД ======================
int Mainform::login(QString login_name,QString password)
{
	if (login_name == "") return false;
	QString table_password;
 //==== проверка логина ====================
	QSqlQuery query;
	QString str = QString("SELECT id_user from users where login_name = '%1'").arg(login_name);
	if(!query.exec(str))
	{
	 return false;
	}
    
	QSqlRecord rec = query.record();
	id_user=0;
	while(query.next())
	{	
		id_user = query.value(rec.indexOf("id_user")).toInt();
	}	
	if (id_user == 0) return 0;
	else
	{
	//======= проверка пароля в БД ========
		str = QString("SELECT passwd from users_passwd where id_user = '%1'").arg(id_user);
		if(!query.exec(str))
		{
		 return 0;
		}
	    
		rec = query.record();
		while(query.next())
		{	
			table_password = query.value(rec.indexOf("passwd")).toString();
		}	
	}
//===============================================
	
	if (table_password == str_to_md5(password))
	{
		return id_user;
	}
	else
	{
		return 0;
	}
//===============================================
}



//============== Шифрование строки по алгоритму md5 ====================
QString Mainform::str_to_md5(QString str)
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(str.toAscii()); 
	QString md5_str(hash.result().toHex());
	
	return md5_str;
}

//====================== Формирование меню в зависимости от статуса пользователя ====================================
void Mainform::create_user_menu(int id_user)
{
	if (!connection_flag) return; //== проверка, есть ли соединение с БД 
	
	//=========== Проверка статуса пользователя по его id_user ============ 
	QSqlQuery query;
	QString str = QString("SELECT id_group from users where id_user = '%1'").arg(id_user);
	if(!query.exec(str))
	{
	 return;
	}
    QSqlRecord rec = query.record();
	int id_user_group;
	while(query.next())
	{	
		id_user_group = query.value(rec.indexOf("id_group")).toInt();
	}	
	//===== Формирование меню в зависимости от статуса пользователя ========
	init_menu(id_user_group);
   
	return;
}




//===================================================
void Mainform::show_user_form()
{
	  ManageUsers *u = new ManageUsers();
      u->show();
	  
}

//================ Форма ввода и редактирования информации ========

//============= Открытие формы работы с картой ==============
void Mainform::show_map_form()
{
	MapView * m = new MapView;
	QMdiSubWindow * mapW = m_mdiArea->addSubWindow (m);
    mapW->setAttribute (Qt::WA_DeleteOnClose);
    m->showMaximized();
	m_mdiArea->setActiveSubWindow (mapW);   

}

//================ Диалог редактирования вспомогательных таблиц =================
void Mainform::show_supporting_tables_form(){
	  SupportingTables *supp = new SupportingTables();
	  supp->setWindowTitle("Редактирование справочников");
	  supp->setModal(true);
      supp->show();
}
//================ Открытие формы управления объектами =================
void Mainform::show_object_manager_form(){
	Objectmanager *obman = new Objectmanager();
	QMdiSubWindow * obmanager = m_mdiArea->addSubWindow (obman);
	obmanager->setAttribute (Qt::WA_DeleteOnClose);
	obman->setWindowTitle("Управление объектами");
	obman->showMaximized();
	m_mdiArea->setActiveSubWindow (obmanager);  

}

void Mainform::show_calculating_form(){
	calc = new CalculatingProblemManager();
	QMdiSubWindow * obmanager = m_mdiArea->addSubWindow (calc);
	obmanager->setAttribute (Qt::WA_DeleteOnClose);
	calc->setWindowTitle("Расчетные задачи");
	calc->showMaximized();
	m_mdiArea->setActiveSubWindow (obmanager);  
}
