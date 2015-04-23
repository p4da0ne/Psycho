#include "manage_users.h"
#include "ui_manage_users_form.h"


ManageUsers::ManageUsers(QDialog *parent)
    : QDialog(parent),
	UI (new Ui::manage_users_form)
{
    UI->setupUi(this);
	view_users();
	connect(UI->add_user_button,SIGNAL(clicked()),this, SLOT(add_user()));
	connect(UI->user_del_button,SIGNAL(clicked()),this,SLOT(del_user()));
	connect(UI->reload_button,SIGNAL(clicked()),this,SLOT(view_users()));
	connect(UI->save_button,SIGNAL(clicked()),this,SLOT(save()));
	connect(UI->reload_password,SIGNAL(clicked()),this,SLOT(ReWrite_password()));
}
QString ManageUsers::str_to_md5(QString str)
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(str.toAscii()); 
	QString md5_str(hash.result().toHex());
	return md5_str;
}

ManageUsers::~ManageUsers()
{
	delete UI;
}

void ManageUsers::add_user()
{
	a_u = new QDialog();
	a_u->setWindowTitle("Добавление нового пользователя");
	Group = new QComboBox();
	Group->addItem(tr("developer"),1);
	Group->addItem(tr("administrator"),2);
	Group->addItem(tr("user"),3);
	
	military_rang = new QComboBox();

	QSqlQuery query;
	QString str = "SELECT id_military_rank,rank_name FROM military_rank";
	query.exec(str);
	QSqlRecord data = query.record();
	QString rang;
	int id_rang;
	int id_rank;
	while(query.next())
	{
		rang = query.value(data.indexOf("rank_name")).toString();
		id_rank = query.value(data.indexOf("id_military_rank")).toInt();
		military_rang->addItem(rang,id_rank);
	}
	military_rang->setCurrentIndex(1);

	Fmaly = new QLabel("Фамилия:");
	Name = new QLabel("Имя:");
	Otchestvo = new QLabel("Отчество:");
	login = new QLabel("Логин:");
	password = new QLabel("Пароль:");
	military_rank = new QLabel("Воинское звание");
	group = new QLabel("Группа:");

	Fmaly_data = new QLineEdit;
	Name_data = new QLineEdit;
	Otchestvo_data = new QLineEdit;
	login_data = new QLineEdit;
	password_data = new QLineEdit;

	ok = new QPushButton("OK");
	cansel = new QPushButton("Exit");
	
	connect(ok,SIGNAL(clicked()),this,SLOT(add_user_ok()));
	connect(cansel,SIGNAL(clicked()),this,SLOT(add_user_exit()));
	QGridLayout *Grid = new QGridLayout;

	Grid->addWidget(Fmaly,1,1);
	Grid->addWidget(Name,2,1);
	Grid->addWidget(Otchestvo,3,1);
	Grid->addWidget(military_rank,4,1);
	Grid->addWidget(group,5,1);
	Grid->addWidget(login,6,1);
	Grid->addWidget(password,7,1);

	Grid->addWidget(Fmaly_data,1,3);
	Grid->addWidget(Name_data,2,3);
	Grid->addWidget(Otchestvo_data,3,3);
	Grid->addWidget(military_rang,4,3);
	Grid->addWidget(Group,5,3);
	Grid->addWidget(login_data,6,3);
	Grid->addWidget(password_data,7,3);
	
	Grid->addWidget(ok,9,1);
	Grid->addWidget(cansel,9,3);

	a_u->setLayout(Grid);
	
	if(a_u->exec() == QDialog::Accepted)
	{
		
	}

}

void ManageUsers::add_user_exit()
{
	a_u->hide();
}

void ManageUsers::add_user_ok()
{
	stac = new QLabel(tr("Error SQL: "));
	error = new QLabel();
	
	QSqlQuery query_add_user;
	QSqlQuery query_add_password;

	QString Fam = Fmaly_data->text();
	QString Name = Name_data->text();
	QString Otch = Otchestvo_data->text();
	QString Login = login_data->text();
	QString Password = password_data->text();

	int Rang_id = military_rang->itemData(military_rang->currentIndex()).toInt();
	int group_id = Group->currentIndex();

	view_users();
	user_id_max = user_id_max + 1;
	Rang_id++;
	group_id++;
	
	
	QTableWidgetItem *item = new QTableWidgetItem;
	int m = UI->users_tabe->rowCount();
	for(int i = 0; i<m; i++)
	{
		if(UI->users_tabe->item(i,6)->text() == Login)
		{
			errors_message(tr("This login already exist!!!"));
			return;
		}
	}
	int control = Login.length();

	if(control<3)
	{
		errors_message(tr("Login must be longer that 2 simvol"));
		return;
	}

	control = Password.length();
	if(control<3)
	{
		errors_message(tr("Password must be longer that 2 simvol"));
		return;
	}

	control = Fam.length();
	if(control<1)
	{
		errors_message(tr("Please, enter you surname"));
		return;
	}


	control = Name.length();
	if(control<1)
	{
		errors_message(tr("Please, enter you name"));
		return;
	}


	control = Otch.length();
	if(control<1)
	{
		errors_message(tr("Please, enter you patronumic"));
		return;
	}

	



	QString query = QString("INSERT INTO users (id_user,id_group,id_military_rank,surname,name,patronumic,login_name) VALUES ('%1','%2','%3','%4','%5','%6','%7')").arg(user_id_max).arg(QString::number(group_id)).arg(QString::number(Rang_id)).arg(Fam).arg(Name).arg(Otch).arg(Login);
	//password_data->setText(str_to_md5(password_data->text()));

	if(!query_add_user.exec(query))
	{	QSqlError error_sql = query_add_user.lastError();
	errors_message(error_sql.text());
		return;
	}
	query = QString("INSERT INTO users_passwd (id_user,passwd) VALUES ('%1','%2')").arg(user_id_max).arg(str_to_md5(Password));
	if(!query_add_password.exec(query))
	{
		QSqlError error_sql = query_add_password.lastError();
	errors_message(error_sql.text());
		return;	
	}
	errors_message("User add in data base");
	//a_u->hide();
	view_users();
}

void ManageUsers::view_users()
{
	QSqlQuery query;
	QString str = "SELECT ug.description,u.surname,u.name,u.patronumic,u.login_name,u.id_user,mk.rank_name,up.passwd FROM users u,users_passwd up,user_group ug,military_rank mk WHERE u.id_group=ug.id_user_group AND u.id_military_rank=mk.id_military_rank AND u.id_user=up.id_user";
	
	if(!query.exec(str))
	{
		QSqlError error_sql = query.lastError();
	errors_message(error_sql.text());
		return;
	}
	QSqlRecord data = query.record();
	QString group_name,surname,name,patronumic,login_name,rank_name,passwd;
	int id_user;
	clear_tableWidget(UI->users_tabe);
	UI->users_tabe->setColumnCount(data.count()+1);
	UI->users_tabe->hideColumn(0);
	int row_count = 0;	
	user_id_max = 0;
	QStringList str_list;
	str_list << tr("id_user") << tr(" ") << tr("Group") << tr("Second_name") << tr("Name") << tr("Third_name") << tr("Login") << tr("Rang") << tr("Password");
	
	UI->users_tabe->setColumnWidth(1,25);
	UI->users_tabe->setColumnWidth(2,150);

	UI->users_tabe->setHorizontalHeaderLabels(str_list);
	while(query.next())
		{	
		id_user = query.value(data.indexOf("id_user")).toInt();

		if(user_id_max<id_user) user_id_max = id_user;
		
		group_name = query.value(data.indexOf("description")).toString();
		surname = query.value(data.indexOf("surname")).toString();
		name = query.value(data.indexOf("name")).toString();
		patronumic = query.value(data.indexOf("patronumic")).toString();
		login_name = query.value(data.indexOf("login_name")).toString();
		rank_name = query.value(data.indexOf("rank_name")).toString();
		passwd = query.value(data.indexOf("passwd")).toString();
		
		cbx = new QCheckBox();

		UI->users_tabe->insertRow(row_count);
		Item = new QTableWidgetItem(QString::number(id_user));
		UI->users_tabe->setItem(row_count,0,Item);
		
		Item = new QTableWidgetItem;
		Item->setData(Qt::CheckStateRole, Qt::Unchecked);
		UI->users_tabe->setItem(row_count,1,Item);

		Item = new QTableWidgetItem(group_name);
		UI->users_tabe->setItem(row_count,2,Item);
		
		Item = new QTableWidgetItem(surname);
		UI->users_tabe->setItem(row_count,3,Item);
		
		Item = new QTableWidgetItem(name);
		UI->users_tabe->setItem(row_count,4,Item);
		
		Item = new QTableWidgetItem(patronumic);
		UI->users_tabe->setItem(row_count,5,Item);
		
		Item = new QTableWidgetItem(login_name);
		UI->users_tabe->setItem(row_count,6,Item);
		
		Item = new QTableWidgetItem(rank_name);
		UI->users_tabe->setItem(row_count,7,Item);
		
		Item = new QTableWidgetItem(passwd);
		UI->users_tabe->setItem(row_count,8,Item);

		
		}	

	UI->users_tabe->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	UI->users_tabe->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
	UI->users_tabe->setColumnWidth(1,25);
}
void ManageUsers::del_user()
{
    int row_count = UI->users_tabe->rowCount();
	int f = 0;
	for(int i=0;i<row_count;i++)
	{
		f = f + UI->users_tabe->item(i,1)->data(Qt::CheckStateRole).toInt();
	}
		if(f > 0)
		{
  			//================MessageBox===============================
			  QMessageBox msgBox;
			  msgBox.setWindowTitle(tr("Warning"));
			  msgBox.setText(tr("Do you want to delete selected rows?"));   
			  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
				 switch (msgBox.exec()) {
				 case QMessageBox::Yes:
					 // yes was clicked
					 break;
				 case QMessageBox::No:
					 return;
					 break;
				 default:
					 return;
					 break;
				 }
		}
		else
		{
			//================MessageBox===============================
			  QMessageBox msgBox;
			  msgBox.setWindowTitle(tr("Message"));
			  msgBox.setText(tr("No one rows selected!"));   
			  msgBox.setStandardButtons(QMessageBox::Ok);
				 switch (msgBox.exec()) {
				 case QMessageBox::Yes:
					 // yes was clicked
					 return;
					 break;
				 }
		}


	int n = UI->users_tabe->rowCount();
	for(int i = 0;i<n;i++)
	{
		if(UI->users_tabe->item(i,1)->data(Qt::CheckStateRole).toBool() == true)
		{
			del_user_run(UI->users_tabe->item(i,0)->text().toInt());
		}
	}
	view_users();
}
void ManageUsers::del_user_run(int id_del_user)
{
	
	QSqlQuery query;
	QString str = QString("DELETE FROM users_passwd WHERE id_user='%1'").arg(id_del_user);
	if(!query.exec(str))
	{
		QSqlError error_sql = query.lastError();
		errors_message(error_sql.text());
		return;
	}
	str = QString("DELETE FROM users WHERE id_user='%1'").arg(id_del_user);
	if(!query.exec(str))
	{
		QSqlError error_sql = query.lastError();
		errors_message(error_sql.text());
		return;
	}
	//errors_message(tr("Successful!!!"));
}
void ManageUsers::errors_message(QString str)
{
	QMessageBox error;
	QMessageBox::warning(this, tr("Warning!!!"),str,
		QMessageBox::Ok);
}
void ManageUsers::save()
{
	QSqlQuery query;
	 for(int i = 0;i<UI->users_tabe->rowCount(); i++)
	  {	
	    QString str = QString("UPDATE users_passwd SET passwd='%2' WHERE id_user='%1'").arg(UI->users_tabe->item(i,0)->text().toInt()).arg(UI->users_tabe->item(i,8)->text());
	    if(!query.exec(str))
	      {
		    QSqlError error_sql = query.lastError();
		    errors_message(error_sql.text());
		    return;	
	      }
	    str = QString("UPDATE users SET surname='%2',name='%3', patronumic='%4',login_name='%5' WHERE id_user='%1'").arg(UI->users_tabe->item(i,0)->text().toInt()).arg(UI->users_tabe->item(i,3)->text()).arg(UI->users_tabe->item(i,4)->text()).arg(UI->users_tabe->item(i,5)->text()).arg(UI->users_tabe->item(i,6)->text());
	    if(!query.exec(str))
	      {
		   QSqlError error_sql = query.lastError();
		   errors_message(error_sql.text());
		   return;	
	      }
	}
}

void ManageUsers::ReWrite_password()
{
	int row_count = UI->users_tabe->rowCount();
	bool fl;
	int f = 0;
	QSqlQuery query;
	QString str;
	
	for(int i=0;i<row_count;i++)
	{
		f = f + UI->users_tabe->item(i,1)->data(Qt::CheckStateRole).toInt();
	}
	    if(f>2)
		{
			  QMessageBox msgBox;
			  msgBox.setWindowTitle(tr("Warning"));
			  msgBox.setText(tr("You must to select only 1 rows"));
			  msgBox.setStandardButtons(QMessageBox::Ok);
				 switch (msgBox.exec()) {
				 case QMessageBox::Yes:
					 // yes was clicked
					 
					 break;
				 }
				 return;
		}

		if(f > 0)
		{
  			//================MessageBox===============================
			  QMessageBox msgBox;
			  msgBox.setWindowTitle(tr("Warning"));
			  msgBox.setText(tr("Do you want to change password in selected rows?"));   
			  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
				 switch (msgBox.exec()) {
				 case QMessageBox::Yes:
					 // yes was clicked
					 break;
				 case QMessageBox::No:
					 return;
					 break;
				 default:
					 return;
					 break;
				 }
		}
		else
		{
			//================MessageBox===============================
			  QMessageBox msgBox;
			  msgBox.setWindowTitle(tr("Message"));
			  msgBox.setText(tr("No one rows selected!"));   
			  msgBox.setStandardButtons(QMessageBox::Ok);
				 switch (msgBox.exec()) {
				 case QMessageBox::Yes:
					 // yes was clicked
					 
					 break;
				 }
				 return;
		}

	pas = new QDialog();
	QGridLayout *Grid = new QGridLayout;
	QLabel *name = new QLabel(QString("Input new password" ));
	pas_data = new QLineEdit();
	QPushButton *Change_PB = new QPushButton("OK");
	connect(Change_PB,SIGNAL(clicked()),pas,SLOT(accept()));
	
	Grid->addWidget(name,1,1);
	Grid->addWidget(pas_data,1,2);
	Grid->addWidget(Change_PB,1,3);
	
	pas->setLayout(Grid);

	int flag;
	int n = UI->users_tabe->rowCount();
	for(int i = 0;i<n;i++)
	{
		if(UI->users_tabe->item(i,1)->data(Qt::CheckStateRole).toBool() == true)
		{
			flag=i;
		}
	}

			
	name->setText(tr("Input new password "));
	if(pas->exec() == QDialog::Accepted)
	{
		str = QString("UPDATE users_passwd SET passwd='%1' WHERE id_user='%2'").arg(str_to_md5(pas_data->text())).arg(UI->users_tabe->item(flag,0)->text().toInt());
			
	   if(!query.exec(str))
			
	   {		
			QSqlError error_sql = query.lastError();
			errors_message(error_sql.text());
			return;
	   }
	   view_users();
	   delete pas;
	
	}

	
			
			

			
			

			
		
	
	

}

//========== Функция очищения таблицы (удаление всех строк и столбцов) ===============
void ManageUsers::clear_tableWidget(QTableWidget *table)
{
	int count_rows = table->rowCount();
	while(count_rows >= 0)
	{
		table->removeRow(count_rows);
		count_rows--;
	}

	int count_cols = table->columnCount();
	while(count_cols >= 0)
	{
		table->removeColumn(count_cols);
		count_cols--;
	}
}

