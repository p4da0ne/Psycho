#include "users_manager.h"
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHeaderView>
#include <QCryptographicHash>

UsersManager::UsersManager(QDialog *parent)
    : QDialog(parent)
{
	setWindowTitle("Управление пользователями");

    addUserButton = new QToolButton;
	addUserButton->setIcon(QIcon(":/Resources/add_but.png"));
	addUserButton->setToolTip("Добавить нового пользователя");

	editUserButton = new QToolButton;
	editUserButton->setIcon(QIcon(":/Resources/edit_but.png"));
	editUserButton->setToolTip("Редактировать данные выбранного пользователя");

	passwordButton = new QToolButton;
	passwordButton->setIcon(QIcon(":/Resources/stock_lock.png"));
	passwordButton->setToolTip("Изменить пароль выбранного пользователя");

	delUserButton = new QToolButton;
	delUserButton->setIcon(QIcon(":/Resources/delete_but.png"));
	delUserButton->setToolTip("Удалить выбранных пользователей");

	connect(addUserButton,SIGNAL(clicked()),this,SLOT(addUser()));
	connect(editUserButton,SIGNAL(clicked()),this,SLOT(editUser()));
	connect(delUserButton,SIGNAL(clicked()),this,SLOT(deleteUser()));
	connect(passwordButton,SIGNAL(clicked()),this,SLOT(changeUserPassword()));

	QHBoxLayout *toolLay = new QHBoxLayout;
	toolLay->addWidget(addUserButton);
	toolLay->addWidget(editUserButton);
	toolLay->addWidget(passwordButton);
	toolLay->addWidget(delUserButton);
	toolLay->addStretch();

	usersView = new QTableView;
	usersModel = new QStandardItemModel;
	usersView->setModel(usersModel);

	usersView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	usersView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	


	QVBoxLayout *v_lay = new QVBoxLayout;
	v_lay->addLayout(toolLay);
	v_lay->addWidget(usersView);
	setLayout(v_lay);
	
	fillUsersModel();
	usersView->resizeColumnsToContents();
	resize(700,400);

}


UsersManager::~UsersManager()
{
}



//===================================================================================
//===== Метод инициализации модели пользователей и отображения её в TableView =======
//===================================================================================
void UsersManager::fillUsersModel()
{
	usersModel->clear();
	int idUser;
	QString rank,surname,name,patronumic,login,groupName;
	
	QSqlQuery query;
	QString str=QString("SELECT u.id_user,m_l.rank_name,u.surname,u.name,u.patronumic,u.login_name,u_g.description \
						 FROM users u, user_group u_g, military_rank m_l \
						 WHERE u.id_group = u_g.id_user_group \
						 AND m_l.id_military_rank = u.id_military_rank \
						 ORDER BY u_g.description, u.surname");
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{		
			idUser = query.value(rec.indexOf("id_user")).toInt();
			rank = query.value(rec.indexOf("rank_name")).toString();
			surname = query.value(rec.indexOf("surname")).toString();
			name = query.value(rec.indexOf("name")).toString();
			patronumic = query.value(rec.indexOf("patronumic")).toString();
			login = query.value(rec.indexOf("login_name")).toString();
			groupName = query.value(rec.indexOf("description")).toString();
			
			QList<QStandardItem*> rowList;

			QStandardItem *item = new QStandardItem;
			item->setData(rank,Qt::DisplayRole);
			item->setCheckable(true);
			item->setCheckState(Qt::Unchecked);
			item->setData(idUser,Qt::UserRole);
			rowList.append(item);

			item = new QStandardItem(surname);
			rowList.append(item);
			
			item = new QStandardItem(name);
			rowList.append(item);
			
			item = new QStandardItem(patronumic);
			rowList.append(item);

			item = new QStandardItem(login);
			rowList.append(item);

			item = new QStandardItem(groupName);
			rowList.append(item);

			usersModel->appendRow(rowList);
		}
		QStringList hList;
		hList<<"Воинское звание"<<"Фамилия"<<"Имя"<<"Отчество"<<"Логин"<<"Статус";
		usersModel->setHorizontalHeaderLabels(hList);
	}
	return;
}

//================================================================
//==== Метод возвращает число выбранных пользователей в модели ===
//================================================================
int UsersManager::selectedUsersCount()
{
	int count = 0;
	for(int row=0;row<usersModel->rowCount();row++)
	{
		if(usersModel->item(row)->checkState() == Qt::Checked)
		{
			count++;
		}
	}
	return count;
}


//==================================================================
//====== Слот добавления нового пользователя =======================
//==================================================================
void UsersManager::addUser()
{
		
	userDlg = new UserDataDialog;
	if(userDlg->exec() == QDialog::Accepted)
	{
		//------ Вставка данных в БД ------
		addUserInDB();
	}
}

//==================================================================
//====== Слот редактирования данных пользователя ===================
//==================================================================
void UsersManager::editUser()
{
	if(selectedUsersCount() != 1)
	{
		showMessageToUser("Для редактирования данных \nнеобходимо выбрать одного пользователя.");
		return;
	}
	int idUser;
	for(int row=0;row<usersModel->rowCount();row++)
	{
		if(usersModel->item(row,0)->checkState() == Qt::Checked)
		{
			idUser = usersModel->data(usersModel->index(row,0),Qt::UserRole).toInt();
		}
	}
	userDlg = new UserDataDialog(idUser);
	if(userDlg->exec() == QDialog::Accepted)
	{
		//------ Обновление данных в БД ------
		updateUserDataInDB();
	}
	//
}

//==================================================================
//====== Слот удаления пользователя ================================
//==================================================================
void UsersManager::deleteUser()
{
	if(selectedUsersCount() == 0)
	{
		showMessageToUser("Для удаления необходимо выбрать \nодного или нескольких пользователей.");
		return;
	}
	//================MessageBox===============================
	QMessageBox msgBox;
	msgBox.setWindowTitle("Предупреждение");
	msgBox.setText("Вы действительно хотите удалить выбранных пользователей?");   
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
	//-----------------------------------------------------------------------------
	deleteSelectedUsers();
}

//===========================================================================
//=== Сообщение пользователю в виде диалогового окна ========================
//===========================================================================
void UsersManager::showMessageToUser(const QString message) 
{
    QMessageBox::information(this, "Сообщение",
                          message,
                          QMessageBox::Ok, 0);
}

//===========================================================================
//====== Метод удаления выбранных пользователей =============================
//===========================================================================
void UsersManager::deleteSelectedUsers()
{
	QSqlQuery query;
	QString str;
	int idUser;

	for(int row=usersModel->rowCount()-1;row>=0;row--)
	{
		if(usersModel->item(row)->checkState() == Qt::Checked)
		{
			idUser = usersModel->data(usersModel->index(row,0),Qt::UserRole).toInt();

			str = QString("DELETE FROM users_passwd u_p WHERE u_p.id_user = %1").arg(idUser);
			if(query.exec(str))
			{
				query.clear();
				str = QString("DELETE FROM users WHERE id_user = %1").arg(idUser);
				if(query.exec(str))
				{
					
				}
			}
		}
	}
	fillUsersModel();
}



//============================================================================
//======= Метод добавления нового пользователя в БД ============
//============================================================================
void UsersManager::addUserInDB()
{
	QString surname, name, patronumic, login, password;
	int idRank, idGroup;

	password = str_to_md5(userDlg->passwordLineEdit.text());
	surname = userDlg->surnameLineEdit.text();
	name = userDlg->nameLineEdit.text();
	patronumic = userDlg->patronLineEdit.text();
	login = userDlg->loginLineEdit.text();
	idRank = userDlg->rankCombo.itemData(userDlg->rankCombo.currentIndex(),Qt::UserRole).toInt();
	idGroup = userDlg->groupCombo.itemData(userDlg->groupCombo.currentIndex(),Qt::UserRole).toInt();

	QSqlQuery query;
	QString str = QString("INSERT INTO users (id_user,id_group,id_military_rank,surname,name,patronumic,login_name) \
						  VALUES (DEFAULT,%1,%2,'%3','%4','%5','%6') RETURNING id_user").arg(idGroup).arg(idRank).arg(surname).arg(name).arg(patronumic).arg(login);
	
	int id_user;

	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		query.next();
		id_user = query.value(rec.indexOf("id_user")).toInt();
	}
	else
	{
		return;
	}

	QSqlQuery queryPass;
	if(queryPass.exec(QString("INSERT INTO users_passwd (id_user, passwd) VALUES (%1, '%2')").arg(id_user).arg(password)))
	{
		fillUsersModel();
		return;
	}
	return;
}


//============================================================================
//======= Метод обновления данных пользователя в БД ============
//============================================================================
void UsersManager::updateUserDataInDB()
{
	QString surname, name, patronumic, login;
	int idRank, idGroup, id_user;

	surname = userDlg->surnameLineEdit.text();
	name = userDlg->nameLineEdit.text();
	patronumic = userDlg->patronLineEdit.text();
	login = userDlg->loginLineEdit.text();
	idRank = userDlg->rankCombo.itemData(userDlg->rankCombo.currentIndex(),Qt::UserRole).toInt();
	idGroup = userDlg->groupCombo.itemData(userDlg->groupCombo.currentIndex(),Qt::UserRole).toInt();
	id_user = userDlg->idUser;

	QSqlQuery query;
	QString str = QString("UPDATE users SET id_group=%1,id_military_rank=%2,surname='%3',name='%4',patronumic='%5',login_name='%6' \
						  WHERE id_user=%7").arg(idGroup).arg(idRank).arg(surname).arg(name).arg(patronumic).arg(login).arg(id_user);
	
	if(query.exec(str))
	{
		fillUsersModel();
	}
	else
	{
		return;
	}
}


//====================================================================
//===== Метод шифрования строки по методу MD5 ========================
//====================================================================
QString UsersManager::str_to_md5(QString str)
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(str.toAscii()); 
	QString md5_str(hash.result().toHex());
	return md5_str;
}

//====================================================================
//====== Слот изменения пароля пользователя ==========================
//====================================================================
void UsersManager::changeUserPassword()
{
	if(selectedUsersCount() != 1)
	{
		showMessageToUser("Для изменения пароля выберите одного пользователя.");
		return;
	}
	int idUser;
	for(int row=0;row<usersModel->rowCount();row++)
	{
		if(usersModel->item(row,0)->checkState() == Qt::Checked)
		{
			idUser = usersModel->data(usersModel->index(row,0),Qt::UserRole).toInt();
		}
	}

	passwordDlg = new PasswordDialog(idUser);
	if(passwordDlg->exec() == QDialog::Accepted)
	{
		QString newPassword = passwordDlg->newPasswordLineEdit.text();
		updateUserPasswordInDB(idUser, str_to_md5(newPassword));
	}

}


//====================================================================================
//====== Метод изменения пароля пользователя в БД ====================================
//====================================================================================
void UsersManager::updateUserPasswordInDB(int idUser,QString newPassword)
{
	QSqlQuery query;
	QString str = QString("UPDATE users_passwd SET passwd = '%1' WHERE id_user = %2").arg(newPassword).arg(idUser);
	
	QString mess;
	if(query.exec(str))
	{
		mess = "Пароль изменен успешно.";
	}
	else
	{
		mess = "При изменении пароля произошла ошибка.";
	}
	showMessageToUser(mess);
	return;
}