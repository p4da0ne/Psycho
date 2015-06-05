#include "users_manager.h"
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHeaderView>

UsersManager::UsersManager(QDialog *parent)
    : QDialog(parent)
{
    usersView = new QTableView;
	usersModel = new QStandardItemModel;
	usersView->setModel(usersModel);
	


	QVBoxLayout *v_lay = new QVBoxLayout;
	v_lay->addWidget(usersView);
	setLayout(v_lay);
	
	fillUsersModel();
	resize(800,500);

}

//QString ManageUsers::str_to_md5(QString str)
//{
//	QCryptographicHash hash(QCryptographicHash::Md5);
//	hash.addData(str.toAscii()); 
//	QString md5_str(hash.result().toHex());
//	return md5_str;
//}

UsersManager::~UsersManager()
{
}



//===================================================================================
//===== Метод инициализации модели пользователей и отображения её в TableView =======
//===================================================================================
void UsersManager::fillUsersModel()
{
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
	}
	return;
}