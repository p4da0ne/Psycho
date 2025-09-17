#include "user_data_dialog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtWidgets/QWidget>


UserDataDialog::UserDataDialog(int idUser, QWidget *parent) :
   QDialog(parent),idUser(idUser)
{
  	resize(300,350);

	rankModel = getRankList();
	groupModel = getGroupList();

	rankCombo.setModel(rankModel);
	groupCombo.setModel(groupModel);

	if(idUser == 0)
	{
        setWindowTitle("Добавление нового пользователя");
	}
	else
	{
		setWindowTitle("Редактирование данных пользователя");
		fillUserData(idUser);
	}


	formLay.addRow("Фамилия:",&surnameLineEdit);
	formLay.addRow("Имя:",&nameLineEdit);
	formLay.addRow("Отчество:",&patronLineEdit);
	formLay.addRow("Воинское звание:",&rankCombo);
	formLay.addRow("Группа:",&groupCombo);
	formLay.addRow("Логин:",&loginLineEdit);
	
	if(this->idUser == 0)
	{
		formLay.addRow("Пароль:",&passwordLineEdit);
		passwordLineEdit.setEchoMode(QLineEdit::Password);
	}
	formLay.setVerticalSpacing(10);

	okButton.setText("Сохранить");
	cancelButton.setText("Отмена");
	buttonLay.addWidget(&okButton);
	buttonLay.addStretch();
	buttonLay.addWidget(&cancelButton);

	dlgLay.addLayout(&formLay);
	dlgLay.addStretch();
	dlgLay.addLayout(&buttonLay);

	setLayout(&dlgLay);
	
	connect(&okButton,SIGNAL(clicked()),this,SLOT(slotAccepted()));
	connect(&cancelButton,SIGNAL(clicked()),this,SLOT(reject()));

}


//== Слот валидации заполненных данных =======
void UserDataDialog::slotAccepted()
{
    QString mess;

	if(surnameLineEdit.text() == "")
    {
		mess.append("Вы не ввели фамилию пользователя.\n");
    }
	if(nameLineEdit.text() == "")
    {
		mess.append("Вы не ввели имя пользователя.\n");
    }
	if(patronLineEdit.text() == "")
    {
		mess.append("Вы не ввели отчество пользователя.\n");
    }
	if(loginLineEdit.text() == "")
    {
		mess.append("Вы не ввели логин пользователя.\n");
    }
	if((this->idUser == 0)&& (passwordLineEdit.text() == ""))
    {
		mess.append("Вы не ввели пароль пользователя.\n");
    }

	if(!isUniqueLogin(loginLineEdit.text()) && (this->idUser == 0))
	{
		mess = "Пользователь таким логином уже зарегистрирован.";
		loginLineEdit.setFocus();
	}

	if(mess != "")
	{
		messageToUser(mess);
		return;
	}
    accept();
}


void UserDataDialog::messageToUser(QString message)
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("Сообщение");
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}


//==================================================================
//======= Метод возвращает список воинских званий из БД ============
//==================================================================
QStandardItemModel* UserDataDialog::getRankList()
{
	QStandardItemModel *model = new QStandardItemModel;
	int idRank;
	QString rankName;

	QSqlQuery query;
	if(query.exec("SELECT id_military_rank,rank_name FROM military_rank ORDER BY id_military_rank"))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{		
			idRank = query.value(rec.indexOf("id_military_rank")).toInt();
			rankName = query.value(rec.indexOf("rank_name")).toString();
			QStandardItem *item = new QStandardItem;
			item->setData(idRank,Qt::UserRole);
			item->setData(rankName,Qt::DisplayRole);
			model->appendRow(item);
		}
	}
	return model;
}


//==================================================================
//======= Метод возвращает список групп пользователей из БД ============
//==================================================================
QStandardItemModel* UserDataDialog::getGroupList()
{
	QStandardItemModel *model = new QStandardItemModel;
	int idGroup;
	QString groupName;

	QSqlQuery query;
	if(query.exec("SELECT id_user_group,description FROM user_group ORDER BY group_name"))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{		
			idGroup = query.value(rec.indexOf("id_user_group")).toInt();
			groupName = query.value(rec.indexOf("description")).toString();
			QStandardItem *item = new QStandardItem;
			item->setData(idGroup,Qt::UserRole);
			item->setData(groupName,Qt::DisplayRole);
			model->appendRow(item);
		}
	}
	return model;
}


//============================================================================
//======= Метод возвращает воинское звание пользователя из БД ================
//============================================================================
QString UserDataDialog::getUserRank(int idUser)
{
	QString rankName;
	QSqlQuery query;
	QString str = QString("SELECT r.rank_name FROM users u, military_rank r \
						   WHERE u.id_military_rank = r.id_military_rank AND u.id_user = %1").arg(idUser);
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{		
			rankName = query.value(rec.indexOf("rank_name")).toString();
		}
	}
	return rankName;
}


//============================================================================
//======= Метод возвращает имя группы пользователя из БД ================
//============================================================================
QString UserDataDialog::getUserGroupName(int idUser)
{
	QString groupName;
	QSqlQuery query;
	QString str = QString("SELECT gr.description FROM users u,user_group gr \
						   WHERE u.id_group=gr.id_user_group AND u.id_user = %1").arg(idUser);
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{		
			groupName = query.value(rec.indexOf("description")).toString();
		}
	}
	return groupName;
}

	
//============================================================================
//======= Метод заполняет поля диалога данными пользователя из БД ============
//============================================================================
void UserDataDialog::fillUserData(int idUser)
{
	QString groupName, rank;
	QSqlQuery query;
	QString str = QString("SELECT u.surname,u.name,u.patronumic,u.login_name \
						   FROM users u WHERE u.id_user = %1").arg(idUser);
	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while (query.next())
		{		
			surnameLineEdit.setText(query.value(rec.indexOf("surname")).toString());
			nameLineEdit.setText(query.value(rec.indexOf("name")).toString());
			patronLineEdit.setText(query.value(rec.indexOf("patronumic")).toString());
			loginLineEdit.setText(query.value(rec.indexOf("login_name")).toString());
		}
	}
	rank = getUserRank(idUser);
	groupName = getUserGroupName(idUser);

	QList<QStandardItem*> items = rankModel->findItems(rank);
	if(items.count() > 0)
	{
		rankCombo.setCurrentIndex(items.at(0)->index().row());
	}
	items.clear();
	items = groupModel->findItems(groupName);
	if(items.count() > 0)
	{
		groupCombo.setCurrentIndex(items.at(0)->index().row());
	}

	return;
}


//===================================================================
//====== Метод проверки уникальности логина в БД ====================
//===================================================================
bool UserDataDialog::isUniqueLogin(QString login)
{
	QString loginName;
	int count = 0;
	QSqlQuery query;
	QString str = QString("SELECT COUNT(id_user) FROM users WHERE login_name = '%1'").arg(login);
	if(query.exec(str))
	{
		query.next();
		count = query.value(0).toInt();
	}
	if(count > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
