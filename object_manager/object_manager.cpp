#include "object_manager.h"
#include <QMenu>
#include <QMouseEvent>
#include <QAction>
#include "../dataaccess/dataaccess.h"
#include <QDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QUrl>
#include <QPainter>
#include <QLineEdit>
#include <QStandardItem>
#include "simledelegate.h"
#include <QProgressDialog>
#include <QStringList>
#include "../reports/reports.h"
#include "ui_add_object.h"
#include "ui_object_manager_form.h"

Objectmanager::Objectmanager(QWidget *parent) //int in_id_object
    : QWidget(parent),
	UI (new Ui::object_manager_form)
{
    UI->setupUi(this);
  	
	delegat= new simleDelegate(UI->property_object); 

	/*
	id_object=in_id_object;
	p_menu=new QMenu(UI->columnView);
	p_menu->addAction(MainCodec->toUnicode("Удалить объект"));
	setWindowIcon(QIcon("./img/database.png"));
	UI->delete_button->setEnabled(false);*/
	UI->object_manager_tree->setContextMenuPolicy(Qt::CustomContextMenu);
	UI->columnView->setContextMenuPolicy(Qt::CustomContextMenu);	
	
	connect(UI->object_manager_tree,SIGNAL(clicked(const QModelIndex &)),this,SLOT(show_objects ( const QModelIndex & )));
	connect(UI->columnView,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(customMenuView(const QPoint &)));
	connect(UI->object_manager_tree,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(customMenuTree(const QPoint &)));
	connect(UI->columnView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(column_item_clicked ( const QModelIndex & )));
	
    iconsList << "./icons/0.png" << "./icons/01.png" << "./icons/02.png" << "./icons/03.png";
	init_object_tree();
}

Objectmanager::~Objectmanager()
{
	delete UI;
}

//======= Формирование списка таблиц управления объектами =======
void Objectmanager::init_object_tree()
{
	QFont font;
	font.setBold(true);
	QString sSql;
	QSqlQuery query;
	query.exec(QString("SELECT id_blok, name_blok FROM BLOK"));
	QSqlRecord rec = query.record();
		QStandardItemModel *model = new QStandardItemModel(this);
		QStandardItem *parentItem = model->invisibleRootItem();	
	
	while (query.next()){
		QStandardItem *item = new QStandardItem(query.value(1).toString());
		int id_blok_type = query.value(0).toInt();
		QString data_type_obj="type_" + QString::number(id_blok_type);
		item->setData(data_type_obj,Qt::UserRole);
		parentItem->appendRow(item);
		
			sSql = (QString("select co.name_country,co.id_country, blc.id_country from blok_country blc,country co where co.id_country=blc.id_country and blc.id_blok=%1").arg(query.value(0).toInt()));
			QSqlQuery childrenQuery;
			if(childrenQuery.exec(sSql)){
				while(childrenQuery.next()){
				QStandardItem *item_1 = new QStandardItem(childrenQuery.value(0).toString());
				QString country_data="country_"+QString::number(childrenQuery.value(1).toInt()) + "_" + QString::number(id_blok_type);
				item_1->setData(country_data,Qt::UserRole);
				item->appendRow(item_1);
				}	
			}
		}
	model->setHeaderData(0, Qt::Horizontal,"Блоки и страны");
	UI->object_manager_tree->setModel(model);

	query.clear();
	
}	

//================== контекстное меню =========================================
void Objectmanager::customMenuView(const QPoint & pos)
{
 QModelIndex index = UI->columnView->currentIndex();
	if(!index.data(Qt::UserRole).toBool()) return;
	QVariant id=index.data(Qt::UserRole);
	if (id.type() == QVariant::String){ 
		QString user_data=id.toString();
		QStringList list=user_data.split("_");
		if(list.value(0)=="region" || list.value(0)=="reg"){
			QPushButton *popupButton = new QPushButton;
			QMenu *menu = new QMenu(this);
			QAction *act=new QAction("Удалить регион",this);
			connect(act,SIGNAL(triggered()),this,SLOT(delete_region()));
            QAction *act_1 = new QAction(QString("Оценка морально-психологической обстановки: %1").arg(list.value(3)),this);
            rez_z_1 = list.value(3).toFloat();
            QAction *otch23 = new QAction(QString("Сформировать отчет"),this);
            groud_id = list.value(1).toInt();
            connect(otch23,SIGNAL(triggered()),this,SLOT(otchet()));

            menu->addAction(act);
            menu->addAction(act_1);
            menu->addAction(otch23);
			popupButton->setMenu(menu);
			menu->exec(QCursor::pos());
            }

		else if(list.value(0)=="dsmi"){
			QPushButton *popupButton = new QPushButton;
			QMenu *menu = new QMenu(this);
			QAction *act=new QAction("Удалить СМИ",this);
			connect(act,SIGNAL(triggered()),this,SLOT(delete_smi()));

            QAction *otch_smi = new QAction (QString("Сформировать отчет"),this);
            smi_id = list.value(1).toInt();
            connect(otch_smi,SIGNAL(triggered()),this,SLOT(otchet_groups()));
			menu->addAction(act);
            menu->addAction(otch_smi);
			popupButton->setMenu(menu);
			menu->exec(QCursor::pos());
			}
		else if(list.value(0)=="dls" ){
			QPushButton *popupButton = new QPushButton;
			QMenu *menu = new QMenu(this);
			QAction *act=new QAction("Удалить воинское формирование",this);
			connect(act,SIGNAL(triggered()),this,SLOT(delete_ls()));
            QAction *otch_ls = new QAction (QString("Сформировать отчет"),this);
            ls_id = list.value(1).toInt();
            connect(otch_ls,SIGNAL(triggered()),this,SLOT(otchet_groups()));


            menu->addAction(act);
            menu->addAction(otch_ls);
			popupButton->setMenu(menu);
			menu->exec(QCursor::pos());
			}
        else if(list.value(0)=="lss" ){//|| list.value(0)=="chls"){
            People_Losses pl;
            pl.get_losses(list.at(1).toInt());

            QPushButton *popupButton = new QPushButton;
			QMenu *menu = new QMenu(this);

            QAction *act=new QAction(QString("Оценка морально-психологического состояния %1: %2").arg(list.value(4)).arg(list.value(3)),this);

            QMenu* subMenu_1 = new QMenu("Потери л/с",menu);

            QMenu* subMenu_1_1 = new QMenu("Интенсивность БД низкая");
            QAction *act_1_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[0][0]).arg(pl.max[0][0]),subMenu_1);
            subMenu_1_1->addAction(act_1_1);
            QAction *act_1_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[0][1]).arg(pl.max[0][1]),subMenu_1);
            subMenu_1_1->addAction(act_1_2);
            QAction *act_1_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[0][2]).arg(pl.max[0][2]),subMenu_1);
            subMenu_1_1->addAction(act_1_3);
            QAction *act_1_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[0][3]).arg(pl.max[0][3]),subMenu_1);
            subMenu_1_1->addAction(act_1_4);
            subMenu_1->addMenu(subMenu_1_1);

            QMenu* subMenu_1_2 = new QMenu("Интенсивность БД средняя");
            QAction *act_2_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[1][0]).arg(pl.max[1][0]),subMenu_1);
            subMenu_1_2->addAction(act_2_1);
            QAction *act_2_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[1][1]).arg(pl.max[1][1]),subMenu_1);
            subMenu_1_2->addAction(act_2_2);
            QAction *act_2_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[1][2]).arg(pl.max[1][2]),subMenu_1);
            subMenu_1_2->addAction(act_2_3);
            QAction *act_2_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[1][3]).arg(pl.max[1][3]),subMenu_1);
            subMenu_1_2->addAction(act_2_4);
            subMenu_1->addMenu(subMenu_1_2);

            QMenu* subMenu_1_3 = new QMenu("Интенсивность БД высокая");
            QAction *act_3_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[2][0]).arg(pl.max[2][0]),subMenu_1);
            subMenu_1_3->addAction(act_3_1);
            QAction *act_3_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[2][1]).arg(pl.max[2][1]),subMenu_1);
            subMenu_1_3->addAction(act_3_2);
            QAction *act_3_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[2][2]).arg(pl.max[2][2]),subMenu_1);
            subMenu_1_3->addAction(act_3_3);
            QAction *act_3_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[2][3]).arg(pl.max[2][3]),subMenu_1);
            subMenu_1_3->addAction(act_3_4);
            subMenu_1->addMenu(subMenu_1_3);

            menu->addMenu(subMenu_1);
            QAction *act_del=new QAction("Удалить воинское формирование",this);
            connect(act_del,SIGNAL(triggered()),this,SLOT(delete_ls()));
            QAction *otch_ls = new QAction (QString("Сформировать отчет"),this);
            ls_id = list.value(1).toInt();
            connect(otch_ls,SIGNAL(triggered()),this,SLOT(otchet_groups()));
            menu->addAction(act);
            menu->addAction(act_del);
            menu->addAction(otch_ls);
            popupButton->setMenu(menu);
            menu->exec(QCursor::pos());

        }
		else if(list.value(0)=="chls"){
			People_Losses pl;
			pl.get_losses(list.at(1).toInt());
			QPushButton *popupButton = new QPushButton;
			QMenu *menu = new QMenu(this);

        //	calculating_mps calc;
            QAction *act=new QAction(QString("Оценка моралогического состояния %1: %2").arg(list.value(3)).arg(list.value(2)),this);
			QAction *act_del=new QAction("Удалить воинское формирование",this);
            connect(act_del,SIGNAL(triggered()),this,SLOT(delete_ls()));
			QMenu* subMenu_1 = new QMenu("Потери л/с",menu);

			QMenu* subMenu_1_1 = new QMenu("Интенсивность БД низкая");
			QAction *act_1_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[0][0]).arg(pl.max[0][0]),subMenu_1);
			subMenu_1_1->addAction(act_1_1);
			QAction *act_1_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[0][1]).arg(pl.max[0][1]),subMenu_1);
			subMenu_1_1->addAction(act_1_2);
			QAction *act_1_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[0][2]).arg(pl.max[0][2]),subMenu_1);
			subMenu_1_1->addAction(act_1_3);
			QAction *act_1_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[0][3]).arg(pl.max[0][3]),subMenu_1);
			subMenu_1_1->addAction(act_1_4);
			subMenu_1->addMenu(subMenu_1_1);

			QMenu* subMenu_1_2 = new QMenu("Интенсивность БД средняя");
			QAction *act_2_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[1][0]).arg(pl.max[1][0]),subMenu_1);
			subMenu_1_2->addAction(act_2_1);
			QAction *act_2_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[1][1]).arg(pl.max[1][1]),subMenu_1);
			subMenu_1_2->addAction(act_2_2);
			QAction *act_2_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[1][2]).arg(pl.max[1][2]),subMenu_1);
			subMenu_1_2->addAction(act_2_3);
			QAction *act_2_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[1][3]).arg(pl.max[1][3]),subMenu_1);
			subMenu_1_2->addAction(act_2_4);
			subMenu_1->addMenu(subMenu_1_2);

			QMenu* subMenu_1_3 = new QMenu("Интенсивность БД высокая");
			QAction *act_3_1=new QAction(QString("через 1 час: %1 - %2").arg(pl.min[2][0]).arg(pl.max[2][0]),subMenu_1);
			subMenu_1_3->addAction(act_3_1);
			QAction *act_3_2=new QAction(QString("втечение суток: %1 - %2").arg(pl.min[2][1]).arg(pl.max[2][1]),subMenu_1);
			subMenu_1_3->addAction(act_3_2);
			QAction *act_3_3=new QAction(QString("через 3-5 суток: %1 - %2").arg(pl.min[2][2]).arg(pl.max[2][2]),subMenu_1);
			subMenu_1_3->addAction(act_3_3);
			QAction *act_3_4=new QAction(QString("более 5 суток: %1 - %2").arg(pl.min[2][3]).arg(pl.max[2][3]),subMenu_1);
			subMenu_1_3->addAction(act_3_4);
			subMenu_1->addMenu(subMenu_1_3);

			menu->addMenu(subMenu_1);

			//QAction *act=new QAction("Удалить воинское формирование",this);
			//connect(act,SIGNAL(triggered()),this,SLOT(delete_ls()));
            QAction *otch_ls = new QAction (QString("Сформировать отчет"),this);
            ls_id = list.value(1).toInt();
            connect(otch_ls,SIGNAL(triggered()),this,SLOT(otchet_groups()));
			menu->addAction(act);
			menu->addAction(act_del);
            menu->addAction(otch_ls);
			popupButton->setMenu(menu);
			menu->exec(QCursor::pos());
			}
		
		else if(list.value(0)=="dgr"){
			QPushButton *popupButton = new QPushButton;
			QMenu *menu = new QMenu(this);
			QAction *act=new QAction("Удалить организацию",this);
			connect(act,SIGNAL(triggered()),this,SLOT(delete_groups()));

            QAction *otch_groups = new QAction (QString("Сформировать отчет"),this);
            group_id = list.value(1).toInt();
            connect(otch_groups,SIGNAL(triggered()),this,SLOT(otchet_groups()));

			menu->addAction(act);
            menu->addAction(otch_groups);
			popupButton->setMenu(menu);
			menu->exec(QCursor::pos());
			}
		else if(list.value(0)=="dsc"){
			QPushButton *popupButton = new QPushButton;
			QMenu *menu = new QMenu(this);
			QAction *act=new QAction("Удалить особое условие",this);
			connect(act,SIGNAL(triggered()),this,SLOT(delete_sc()));
			menu->addAction(act);
			popupButton->setMenu(menu);
			menu->exec(QCursor::pos());
			}
		else if(list.value(0)=="dmpo" || list.value(0)=="dmpos" || list.value(0)=="dmposmi" ){
			QPushButton *popupButton = new QPushButton;
			QMenu *menu = new QMenu(this);
			QAction *act=new QAction("Удалить средство",this);
			connect(act,SIGNAL(triggered()),this,SLOT(delete_mpo()));
			menu->addAction(act);
			popupButton->setMenu(menu);
			menu->exec(QCursor::pos());
			}
	}
}

void Objectmanager::customMenuTree(const QPoint & pos)
{
	QModelIndex index = UI->object_manager_tree->currentIndex();
	if(!index.data(Qt::UserRole).toBool()) return;
	QVariant id=index.data(Qt::UserRole);
	if (id.type() == QVariant::String){ 
		QString user_data=id.toString();
		QStringList list=user_data.split("_");
		if(list.value(0)=="type"){
			int id_type = list.value(1).toInt();
			QPushButton *popupButton = new QPushButton;
			QMenu *menu = new QMenu(this);
			QAction *act=new QAction("Добавить страну в блок",this);
			connect(act,SIGNAL(triggered()),this,SLOT(add_country_blok()));
			menu->addAction(act);
			popupButton->setMenu(menu);
			menu->exec(QCursor::pos());
			}
		else if(list.value(0)=="country"){
			QPushButton *popupButton = new QPushButton;
			QMenu *menu = new QMenu(this);
			QAction *act=new QAction("Удалить страну из блока",this);
			connect(act,SIGNAL(triggered()),this,SLOT(delete_country_blok()));
			menu->addAction(act);
			popupButton->setMenu(menu);
			menu->exec(QCursor::pos());
			}
		}
}
//========================== комбобокс страны =============================
void Objectmanager::fill_combobox_country(QComboBox *box)
{
	box->clear();
	QSqlQuery query;
	box->addItem("-");
	query.exec("SELECT country.id_country,country.name_country FROM country ORDER BY name_country");

	while (query.next()){
	
		QString country_obj = query.value(1).toString();
		int id_country=query.value(0).toInt();
		box->addItem(country_obj,id_country);
		}
	query.clear();
}
//========================== комбобокс блоки =============================
void Objectmanager::fill_combobox_blok(QComboBox *box, int id_current_blok)
{
	box->clear();
	QSqlQuery query;
	query.exec("SELECT blok.id_blok,blok.name_blok FROM blok ORDER BY name_blok");
	int current_index;
	int index=0;
	while (query.next()) 
	{
		QString blok_obj = query.value(1).toString();
		int id_blok=query.value(0).toInt();
		box->addItem(blok_obj,id_blok);
		if(id_current_blok==id_blok){
			current_index=index;
		}
		index++;
	}
	box->setCurrentIndex(current_index);
	query.clear();
	
}

//========================= добавление страны в блок ==============================
void Objectmanager::add_country_blok(){
	
	QDialog *add_element = new QDialog();
	
	QModelIndex index = UI->object_manager_tree->currentIndex();
	QString id=index.data(Qt::UserRole).toString();
	QStringList list=id.split("_");
	add_element->setWindowTitle("Добвление страны в блок");

	QLabel *element_name_blok = new QLabel("Блок:");
	QLabel *element_name_country = new QLabel("Страна:");
 
	QComboBox *blok_combo = new QComboBox();
	QComboBox *country_combo = new QComboBox();

	blok_combo->setFixedWidth(199);
	country_combo->setFixedWidth(199);

	fill_combobox_blok(blok_combo,list.value(1).toInt());
 	fill_combobox_country(country_combo);
 
	QHBoxLayout *element_name_blok_layout = new QHBoxLayout();
	element_name_blok_layout->addWidget(element_name_blok);
	element_name_blok_layout->addWidget(blok_combo);
 
	QHBoxLayout *BP_count_layout = new QHBoxLayout();
	BP_count_layout->addWidget(element_name_country);
	BP_count_layout->addWidget(country_combo);

	QPushButton *ok_button = new QPushButton("Добавить");
	connect(ok_button,SIGNAL(clicked()),add_element,SLOT(accept()));
	QPushButton *cancel_button = new QPushButton("Отмена");
	connect(cancel_button,SIGNAL(clicked()),add_element,SLOT(close()));

	QHBoxLayout *buttons_layout = new QHBoxLayout();
	buttons_layout->addStretch();
	buttons_layout->addWidget(ok_button);
	buttons_layout->addWidget(cancel_button);

	QVBoxLayout *vert_layout = new QVBoxLayout;
	vert_layout->addLayout(element_name_blok_layout);
	vert_layout->addLayout(BP_count_layout);
	vert_layout->addSpacing(30);
	vert_layout->addLayout(buttons_layout);

	add_element->setLayout(vert_layout);

	if(add_element->exec() == QDialog::Accepted){ 
	 if (country_combo->currentIndex() == 0){
		QMessageBox::StandardButton ret; 
		ret = QMessageBox::critical (this,"Ошибка",("Выберите страну "),QMessageBox::Ok );
	}
	
//=============== По кнопке Добавить добавление в БД нового уязвимого элемента ===========
	
	int id_blok = blok_combo->itemData(blok_combo->currentIndex()).toInt();
	int id_country = country_combo->itemData(country_combo->currentIndex()).toInt();
	int id_country_1;

	QSqlQuery query_1;
	query_1.exec(QString("SELECT id_country FROM blok_country where id_blok=%1").arg(id_blok));

	while (query_1.next()){ 
	
		id_country_1=query_1.value(0).toInt();
	
		if (id_country == id_country_1){ 
	
		QMessageBox::StandardButton ret; 
		ret = QMessageBox::critical (this,"Ошибка",("Страна присутствует в блоке "),QMessageBox::Ok );
		return;
		}
	}
	query_1.clear();

	QSqlQuery query;
	 query.prepare("INSERT INTO blok_country (id_blok,id_country) VALUES (?,?)");
	 query.addBindValue(id_blok);
	 query.addBindValue(id_country);
	
		 if(!query.exec()){
		 
		 return;
		 }

	 init_object_tree();
	 return;
 }

}
//===================== Удаление страны из блока ============================
void Objectmanager::delete_country_blok()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Предупреждение");
	msgBox.setText("Вы действительно хотите удалить страну из блока?"); 
	
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, "Да");
	msgBox.setButtonText(QMessageBox::No, "Нет");
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

	QSqlQuery query;

	QModelIndex index = UI->object_manager_tree->currentIndex();
	if(!index.data(Qt::UserRole).toBool()) return;
	QString id_country=index.data(Qt::UserRole).toString();
	QStringList list=id_country.split("_");
	
	QString str = QString("DELETE FROM blok_country WHERE id_country = %1 and id_blok=%2").arg(list.value(1)).arg(list.value(2));
	if(!query.exec(str)){
	 return;
		}
	
	init_object_tree();
	return;

}
//================= выбор объекта из дерева =====================================
void Objectmanager::show_objects(const QModelIndex &index)
{
	UI->coord_object->setModel(0);
	UI->property_object->setModel(0);
	QFont font;
	font.setBold(true);
	QVariant id=index.data(Qt::UserRole);
	if (id.type() == QVariant::String) { 
	QString user_data=id.toString();
	QProgressDialog progress("Формирование информации о регионах", "Отмена", 0, 4, this);
    progress.setWindowModality(Qt::WindowModal);
	progress.setWindowTitle("Формирование информации о регионах");
	progress.show();
    progress.setValue(1);
	
	QStringList list=user_data.split("_");
		if(list.value(0)=="country") {
		int id_country = list.value(1).toInt();
		QSqlQuery query;
		query.exec(QString("SELECT id_region, name_region,parent_region FROM region WHERE id_country=%1 order by name_region").arg(id_country));
		model = new QStandardItemModel(this);
		QStandardItem *parentItem = model->invisibleRootItem();
		
		while (query.next()) {
			
			QSqlQuery query_count; // подсчет ********
			query_count.exec(QString("select count(name_region) from region where parent_region = %1").arg(query.value(0).toInt()));
			while (query_count.next()) {
			int g = query_count.value(0).toInt(); //********
			int id_region=query.value(0).toInt();
			QIcon icon = QIcon(iconsList.at(calc_info_for_region(query.value(0).toString())));		
			QStandardItem *item = new QStandardItem(query.value(1).toString() + " ["  + QString::number(g) + "]");
			item->setIcon(icon);
			QString data_region="region_" + QString::number(id_region) + "_" + QString::number(id_country) + "_" + QString::number(calc.get_Rez_on_id_region(id_region));
			item->setData(data_region,Qt::UserRole);
			//item->setData(QIcon(set_icon(query.value(2).toInt())),Qt::DecorationRole);
			model->appendRow(item);
			child_region_objects(item,id_region);
		}
			
			}
	
		progress.setValue(2);
	QStandardItem *item = new QStandardItem(QIcon("./icons/add.png"),"Добавить регион");
	item->setFont(font);
	item->setData(QString("pregion_%1").arg(id_country),Qt::UserRole);
	progress.setValue(3);
	model->appendRow(item);
	
//	model->sort(2,Qt::AscendingOrder);
	UI->columnView->setModel(model);

	progress.setValue(4);
	progress.close();
		}
	return;
	}
}

void Objectmanager::child_region_objects(QStandardItem *parent_item,int id_parent_region)
{
	//select all Region 
	
	QSqlQuery query;
	int row=0;
	query.exec(QString("SELECT id_region, name_region, parent_region FROM region WHERE parent_region=%1 ORDER BY name_region").arg(id_parent_region));

	if (query.size() == 0)
	{
		
	}
	else
	{
		while (query.next())
		{
			QSqlQuery query_count;
			query_count.exec(QString("select count(name_region) from region where parent_region = %1 ").arg(query.value(0).toInt()));
			while (query_count.next()) {
			int g = query_count.value(0).toInt();
			int id_object=query.value(0).toInt();
			if (id_object == id_parent_region)continue;		
			QString icon = iconsList.at(calc_info_for_region(query.value(0).toString()));			
			child_region_objects(set_child_item(query.value(1).toString() + " [" + QString::number(g)+ "]",
												"reg_" + QString::number(id_object) + "_" + QString::number(id_parent_region) + "_" + QString::number(calc.get_Rez_on_id_region(id_object)),  
												parent_item,
												row,icon),id_object);
												//set_icon(query.value(2).toInt())),	
			row++;
			}
		}
	}
	QFont font;
	font.setBold(true);
	
	set_child_item("Добавить регион",QString("preg_%1").arg(id_parent_region),parent_item,row,"./icons/add.png" ,font);
	add_region_components(parent_item,id_parent_region,row);
	query.clear();
}
void Objectmanager::add_region_components(QStandardItem *parent_item,int id_parent_region,int start_row){
//СМИ ====================================================================================================
	QSqlQuery query;
    query.exec(QString("SELECT sm.id_region,sm.id_smi,sm.id_smi_region,poz.id_position_smi,poz.id_smi FROM smi_region sm,smi poz WHERE id_region=%1 AND sm.id_smi = poz.id_smi").arg(id_parent_region));
	int row=start_row+1;
	int row_sw=0;
//SELECT sm.id_region,sm.id_smi,sm.id_smi_region,poz.id_position_smi,poz.id_smi FROM smi_region sm,smi poz WHERE id_region=%1 AND sm.id_smi = poz.id_smi
	int g; 
	QSqlQuery query_count;
	query_count.exec(QString("SELECT count(id_smi) FROM smi_region WHERE id_region=%1").arg(id_parent_region));
	while (query_count.next()){
		g = query_count.value(0).toInt();}
	
	QStandardItem *item=set_child_item("СМИ ["  + QString::number(g) + "]","smi",parent_item,row,"./icons/printer.png");
	//QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
	row++;
	
	if (query.size() != 0)
	{//	ветка СМИ
		row_sw=0;
		while (query.next())
		{
			QSqlQuery query_smi;
			int id_smi=query.value(1).toInt();
			int id_smi_region=query.value(2).toInt();
            int id_poz_smi = query.value(3).toInt();
			query_smi.exec(QString("SELECT id_smi,name_smi FROM smi WHERE id_smi=%1").arg(query.value(1).toInt()));
			while (query_smi.next())
			{
                if (id_poz_smi == 3 || id_poz_smi == 5) {
                QBrush b(Qt::blue);
            child_smi_objects(set_child_item(query_smi.value(1).toString(),QString("dsmi_%1").arg(id_smi_region) + "_" + QString::number(id_smi),item,row_sw,b),
														id_smi);
                }
                else if (id_poz_smi == 1 || id_poz_smi == 6){
                QBrush b(Qt::red);
            child_smi_objects(set_child_item(query_smi.value(1).toString(),QString("dsmi_%1").arg(id_smi_region) + "_" + QString::number(id_smi),item,row_sw,b),
                                                            id_smi);

                }
                else{
                    QBrush b(Qt::black);
                child_smi_objects(set_child_item(query_smi.value(1).toString(),QString("dsmi_%1").arg(id_smi_region) + "_" + QString::number(id_smi),item,row_sw,b),
                                                                id_smi);
                }
			}
			row_sw++;
		}
	 
	}
	QFont font;
	font.setBold(true);
	set_child_item("Добавить CМИ",QString("psmi_%1").arg(id_parent_region),item,row_sw);
	query.clear();
//Воинские формирования =============================================================================================
	query.exec(QString("SELECT id_region, id_ls,name_ls,enimy_ls FROM ls WHERE id_region=%1 ORDER BY name_ls").arg(id_parent_region));
	int row_vf=0;
	int b; 
	QSqlQuery query_count_ls;
	query_count_ls.exec(QString("SELECT count(id_ls) FROM ls WHERE id_region=%1").arg(id_parent_region));
	while (query_count_ls.next()){
		b = query_count_ls.value(0).toInt();
	}
	item=set_child_item("ВОИНСКИЕ ФОРМИРОВАНИЯ ["  + QString::number(b) + "]",QString("ls_%1").arg(id_parent_region),parent_item,row,"./icons/weapon.png");
	row++;
	if (query.size() != 0)
	{//Ветка воинские формирования
		row_vf=0;
		while (query.next())
		{
			int id_ls=query.value(1).toInt();
			int id_region = query.value(0).toInt();
			bool enemy_ls = query.value(3).toBool();
			float mps_for_ls = calc_mps.calculating(id_ls);
			
			if (enemy_ls == true) {
			QBrush b(Qt::blue);
            child_ls_objects(set_child_item(query.value(2).toString(),QString("chls_%1_%2_%3").arg(id_ls).arg(mps_for_ls).arg(calc_mps.isEnemy(id_ls)),item,row_vf,iconsList.at(calc_mps_for_ls(mps_for_ls)),b),
														id_ls);
			}
			else {
			QBrush b(Qt::red);
            child_ls_objects(set_child_item(query.value(2).toString(),QString("chls_%1_%2_%3").arg(id_ls).arg(mps_for_ls).arg(calc_mps.isEnemy(id_ls)),item,row_vf,iconsList.at(calc_mps_for_ls(mps_for_ls)),b),
														id_ls);
			}
			row_vf++;
		}
	}
	
	set_child_item("Добавить Воинские формирования",QString("pls_%1").arg(id_parent_region),item,row_vf);
	query.clear();
//Организации =============================================================================================
	query.exec(QString("SELECT id_region, id_groups,name_groups FROM groups WHERE id_region=%1").arg(id_parent_region));
	int row_gr=0;

	int a; 
	QSqlQuery query_count_gr;
	query_count_gr.exec(QString("SELECT count(id_groups) FROM groups WHERE id_region=%1").arg(id_parent_region));
	
	while (query_count_gr.next()){
		a = query_count_gr.value(0).toInt();}
	
	item=set_child_item("ОРГАНИЗАЦИИ ["  + QString::number(a) + "]","gr",parent_item,row,"./icons/group.png");

	//item = set_child_item("ОРГАНИЗАЦИИ","gr",parent_item,row,"./icons/group.png");
	row++;
	if (query.size() != 0)
	{//Ветка организации
		row_gr=0;
		while (query.next())
		{
			QSqlQuery query_gr;
			query_gr.exec(QString("SELECT id_groups,name_groups FROM groups WHERE id_groups=%1").arg(query.value(1).toInt()));
			while (query_gr.next())
			{
				int id_gr=query_gr.value(0).toInt();

				child_groups_objects(set_child_item(query_gr.value(1).toString(),QString("dgr_%1").arg(id_gr),item,row_gr),
														id_gr);
				
//				(set_child_item(query_gr.value(1).toString(),QString("dgr_%1").arg(query_gr.value(0).toInt()),item,row_gr);

			}
			row_gr++;
		}
	}
	set_child_item("Добавить Организации",QString("pgr_%1").arg(id_parent_region),item,row_gr);
	query.clear();
//Условия=============================================================================================
query.exec(QString("SELECT id_region, id_special_conditions,name_special_conditions FROM special_conditions WHERE id_region=%1 ORDER BY name_special_conditions").arg(id_parent_region));
	int row_sc=0;

	int y; 
	QSqlQuery query_count_sc;
	query_count_sc.exec(QString("SELECT count(id_special_conditions) FROM special_conditions WHERE id_region=%1").arg(id_parent_region));
	
	while (query_count_sc.next()){
		y = query_count_sc.value(0).toInt();}
	
	item=set_child_item("ОСОБЫЕ УСЛОВИЯ ["  + QString::number(y) + "]","gr",parent_item,row,"./icons/stop2.png");

	//item = set_child_item("ОРГАНИЗАЦИИ","gr",parent_item,row,"./icons/group.png");
	row++;
	if (query.size() != 0)
	{//Ветка организации
		row_sc=0;
		while (query.next())
		{
			QSqlQuery query_sc;
			query_sc.exec(QString("SELECT id_special_conditions,name_special_conditions FROM special_conditions WHERE id_special_conditions=%1 ").arg(query.value(1).toInt()));
			while (query_sc.next())
			{
				int id_sc=query_sc.value(0).toInt();

			//	child_groups_objects(set_child_item(query_sc.value(1).toString(),QString("dsc_%1").arg(id_sc),item,row_sc),
				//										id_sc);
			set_child_item(query_sc.value(1).toString(),QString("dsc_%1").arg(query_sc.value(0).toInt()),item,row_sc);	

			}
			row_sc++;
		}
	}
    set_child_item("Добавить Особые условия",QString("psc_%1").arg(id_parent_region),item,row_sc);
	query.clear();
//========================= Национальность =========================================================
	query.exec(QString("SELECT id_nations,id_region,id_ls_nations FROM ls_nations WHERE id_region=%1").arg(id_parent_region));
//	int row=start_row+1;
	int row_nat=0;

	item=set_child_item("Национальный состав",QString("nations_%1").arg(id_parent_region),parent_item,row,font);
	row++;
	int id_nations;
	int id_ls_nations;
	if (query.size() != 0)
	{row_nat=0;
		while (query.next())
		{
			int id_nations=query.value(0).toInt();
			int id_ls_nations=query.value(2).toInt();
		row_nat++;
		}
	int r =id_nations; 
	int s = id_ls_nations;
	}
	query.clear();
//============================ Религия =============================================================
query.exec(QString("SELECT id_confessions,id_region,id_ls_confessions FROM ls_confessions WHERE id_region=%1").arg(id_parent_region));

	int row_conf=0;
	item=set_child_item("Религиозный состав",QString("confess_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
	if (query.size() != 0)
	{row_conf=0;
		while (query.next())
		{
		//	int id_conf=query.value(0).toInt();
		//	int id_ls_conf=query.value(2).toInt();
		row_conf++;
		}
	}
	query.clear();
//============================ Профессиональный состав =============================================================
query.exec(QString("SELECT id_profession,id_region,id_pop_profession FROM pop_profession WHERE id_region=%1").arg(id_parent_region));

	int row_prof=0;
	item=set_child_item("Профессиональный состав",QString("profess_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
	if (query.size() != 0)
	{row_prof=0;
		while (query.next())
		{
		row_prof++;
		}
	}
	query.clear();
	//============================ Возрастной состав =============================================================
query.exec(QString("SELECT id_age,id_region,id_pop_age FROM pop_age WHERE id_region=%1").arg(id_parent_region));

	int row_age=0;
	item=set_child_item("Возрастной состав",QString("agess_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
	if (query.size() != 0)
	{row_age=0;
		while (query.next())
		{
	
		row_age++;
		}
	}
	query.clear();
	//============================ Половой состав =============================================================
query.exec(QString("SELECT id_region,id_pop_sex FROM pop_sex WHERE id_region=%1").arg(id_parent_region));

	int row_sex=0;
	item=set_child_item("Половой состав",QString("sexss_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
	if (query.size() != 0)
	{row_sex=0;
		while (query.next())
		{
	
		row_sex++;
		}
	}
	query.clear();
//==========================================================================================================================
}	
void Objectmanager::column_item_clicked ( const QModelIndex &index){
	
	UI->coord_object->setModel(0);
	UI->property_object->setModel(0);
	QVariant id=index.data(Qt::UserRole);
	if (id.type() == QVariant::String) 
	{
		QString user_data=id.toString();
		QStringList list=user_data.split("_");
		if(list.value(0)=="pregion"){
			add_object *reg = new add_object(list.value(1).toInt(),true);
			reg->setModal(true);
			reg->exec();
		}
		else if(list.value(0)=="preg"){
			add_object *reg = new add_object(list.value(1).toInt(),false);
			reg->setModal(true);
			reg->exec();
		}
		else if(list.value(0)=="psmi"){
			//вызов диалога добавления СМИ на объекте
			Add_elements_dialog *add_element= new Add_elements_dialog(1,list.value(1).toInt());
			add_element->setModal(true);
		 	int result=add_element->exec();
			//вернул id 
			if (result==0)return;
			//обновление модели, добавление нового item-а
			QSqlQuery query_smi;
			query_smi.exec(QString("SELECT s.id_smi,s.name_smi FROM smi_region r,smi s WHERE r.id_smi_region=%1 and s.id_smi=r.id_smi").arg(result));
			while (query_smi.next())
			{
			QString name_smi=query_smi.value(1).toString();
			QString user_data_smi=QString("dsmi_%1_%2").arg(QString::number(result)).arg(query_smi.value(0).toInt());

			QStandardItem *par = model->itemFromIndex(index)->parent();
			int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
			int r2 = model->itemFromIndex(index)->parent()->rowCount();
				//добавляем в модель новый элемент, затирая "Добавить СМИ"
			child_smi_objects(set_child_item(name_smi,user_data_smi,par,r1),result);

				//вновь добавляем элемент "Добавить СМИ"
			set_child_item("Добавить СМИ",QString("psmi_%1").arg(list.value(1)),par,r2);
		
			}
		} 
			//воинские формирования
		else if(list.value(0)=="pls"){
		
			Add_elements_dialog *add_element= new Add_elements_dialog(2,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setMinimumSize(QSize(500,300));
			add_element->setMaximumSize(QSize(500,300));
			int result=add_element->exec();

			if (result==0)return;
			QSqlQuery query_ls;
			query_ls.exec(QString("SELECT id_ls,name_ls,enimy_ls FROM ls WHERE id_ls=%1").arg(result));
			while (query_ls.next()){
			QString name_ls=query_ls.value(1).toString();
			QString user_data_ls=QString("dls_%1").arg(query_ls.value(0).toInt());
			bool enemy_ls = query_ls.value(2).toBool();
			if (enemy_ls == true) {
				QBrush b(Qt::blue);
				QStandardItem *par = model->itemFromIndex(index)->parent();
				int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
				int r2 = model->itemFromIndex(index)->parent()->rowCount();
					
					child_ls_objects(set_child_item(name_ls,user_data_ls,par,r1,b),result);
					
					set_child_item("Добавить воинское формирование",QString("pls_%1").arg(list.value(1)),par,r2);
			}
			else{
			QBrush b(Qt::red);
				QStandardItem *par = model->itemFromIndex(index)->parent();
				int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
				int r2 = model->itemFromIndex(index)->parent()->rowCount();
				//
				child_ls_objects(set_child_item(name_ls,user_data_ls,par,r1,b),result);
				//
				set_child_item("Добавить воинское формирование",QString("pls_%1").arg(list.value(1)),par,r2);
			
			}
			}
		}
		// подчиненные воинские формирования
		else if(list.value(0)=="plss"){
		
			Add_elements_dialog *add_element= new Add_elements_dialog(5,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setMinimumSize(QSize(500,300));
			add_element->setMaximumSize(QSize(500,300));
			int result=add_element->exec();
						
			QFont font;
			font.setBold(true);
			
			if (result==0)return;
 	       	QSqlQuery query_ls;
			query_ls.exec(QString("SELECT id_ls,name_ls,enimy_ls FROM ls WHERE id_ls=%1").arg(result));
			while (query_ls.next()){
			QString name_ls=query_ls.value(1).toString();
			QString user_data_ls=QString("lss_%1").arg(query_ls.value(0).toInt());
			bool enemy_ls = query_ls.value(2).toBool();
			QBrush b;
			if (enemy_ls == true) {
					b.setColor(Qt::blue);
			}else{
				b.setColor(Qt::red);
			}
			QStandardItem *par = model->itemFromIndex(index)->parent();
			int r7 = model->itemFromIndex(index)->parent()->rowCount()-6;
			int r1 = model->itemFromIndex(index)->parent()->rowCount()-5;
			int r2 = model->itemFromIndex(index)->parent()->rowCount()-4;  
			int r3 = model->itemFromIndex(index)->parent()->rowCount()-3;   
			int r4 = model->itemFromIndex(index)->parent()->rowCount()-2;  
			int r5 = model->itemFromIndex(index)->parent()->rowCount()-1;  
			int r6 = model->itemFromIndex(index)->parent()->rowCount(); 

			QStandardItem *par_i=par->takeChild(r1);
			QStandardItem *par_y=par->takeChild(r2);
			child_ls_objects(set_child_item(name_ls,user_data_ls,par,r7,b),result);
			set_child_item("Добавить подчиненные ВФ",QString("plss_%1").arg(list.value(1)),par,r1);
			par->setChild(r2,par_i);
			par->setChild(r3,par_y);
			set_child_item("Национальный состав",QString("nationss_%1").arg(list.value(1)),par,r4,font);
			set_child_item("Религиозный состав",QString("confesss_%1").arg(list.value(1)),par,r5,font);
			set_child_item("Штатно-должностной состав",QString("rankss_%1").arg(list.value(1)),par,r6,font);				
			}

		}
		// ================  организации
		else if(list.value(0)=="pgr"){
			QFont font;
			font.setBold(true);
		
			Add_elements_dialog *add_element= new Add_elements_dialog(3,list.value(1).toInt());
			add_element->setModal(true);
			int result=add_element->exec();

			if (result==0)return;
            QSqlQuery query_group;
			query_group.exec(QString("SELECT id_groups,name_groups FROM groups WHERE id_groups=%1").arg(result));
			while (query_group.next()){
			QString name_group=query_group.value(1).toString();
			QString user_data_group=QString("dgr_%1").arg(query_group.value(0).toInt());
				QStandardItem *par = model->itemFromIndex(index)->parent();
				int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
				int r2 = model->itemFromIndex(index)->parent()->rowCount();
						
				child_groups_objects(set_child_item(name_group,user_data_group,par,r1),result);	
			
				set_child_item("Добавить организацию",QString("pgr_%1").arg(list.value(1)),par,r2);
			}
		}
		// ================  условия =========================================================
		else if(list.value(0)=="psc"){
			QFont font;
			font.setBold(true);
		
			Add_elements_dialog *add_element= new Add_elements_dialog(16,list.value(1).toInt());
			add_element->setModal(true);
			int result=add_element->exec();

			if (result==0)return;
			QSqlQuery query_scond;
			query_scond.exec(QString("SELECT id_special_conditions,name_special_conditions FROM special_conditions WHERE id_special_conditions=%1").arg(result));
			while (query_scond.next()){
			QString name_sc=query_scond.value(1).toString();
			QString user_data_sc=QString("dsc_%1").arg(query_scond.value(0).toInt());
				QStandardItem *par = model->itemFromIndex(index)->parent();
				int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
				int r2 = model->itemFromIndex(index)->parent()->rowCount();
						
				set_child_item(name_sc,user_data_sc,par,r1);	
			
				set_child_item("Добавить Особые условия",QString("psc_%1").arg(list.value(1)),par,r2);
			}
		}
	// ==================================== средства ======================================
		else if(list.value(0)=="pmpo"){
			
			Add_elements_dialog *add_element= new Add_elements_dialog(13,list.value(1).toInt());
			add_element->setModal(true);
		 	int result=add_element->exec();
			//вернул id 
			if (result==0)return;
			
			QSqlQuery query_mpo;
			query_mpo.exec(QString("SELECT mpo.id_mpo_pso,mpo.name_mpo_pso FROM mpo_pso mpo WHERE id_mpo_pso=%1").arg(result));
			while (query_mpo.next())
			{
			QString name_mpo=query_mpo.value(1).toString();
			QString user_data_mpo=QString("dmpo_%1_%2").arg(QString::number(result)).arg(query_mpo.value(0).toInt());

			QStandardItem *par = model->itemFromIndex(index)->parent();
			int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
			int r2 = model->itemFromIndex(index)->parent()->rowCount();

			set_child_item(name_mpo,user_data_mpo,par,r1);
			
			set_child_item("Добавить Средства",QString("pmpo_%1").arg(list.value(1)),par,r2);
		
			}
		}
		else if(list.value(0)=="pmpos"){
			
			Add_elements_dialog *add_element= new Add_elements_dialog(14,list.value(1).toInt());
			add_element->setModal(true);
            int result=add_element->exec();
			//вернул id 
			if (result==0)return;
			
			QSqlQuery query_mpo;
			query_mpo.exec(QString("SELECT mpo.id_mpo_pso,mpo.name_mpo_pso FROM mpo_pso mpo WHERE id_mpo_pso=%1").arg(result));
			while (query_mpo.next())
			{
			QString name_mpo=query_mpo.value(1).toString();
			QString user_data_mpo=QString("dmpos_%1_%2").arg(QString::number(result)).arg(query_mpo.value(0).toInt());

			QStandardItem *par = model->itemFromIndex(index)->parent();
			int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
			int r2 = model->itemFromIndex(index)->parent()->rowCount();

			set_child_item(name_mpo,user_data_mpo,par,r1);
			
			set_child_item("Добавить Средства",QString("pmpos_%1").arg(list.value(1)),par,r2);
		
			}
		}
		else if(list.value(0)=="pmposmi"){
			
			Add_elements_dialog *add_element= new Add_elements_dialog(15,list.value(1).toInt());
			add_element->setModal(true);
		 	int result=add_element->exec();
			//вернул id 
			if (result==0)return;
			
			QSqlQuery query_mpo;
			query_mpo.exec(QString("SELECT mpo.id_mpo_pso,mpo.name_mpo_pso FROM mpo_pso mpo WHERE id_mpo_pso=%1").arg(result));
			while (query_mpo.next())
			{
			QString name_mpo=query_mpo.value(1).toString();
			QString user_data_mpo=QString("dmposmi_%1_%2").arg(QString::number(result)).arg(query_mpo.value(0).toInt());

			QStandardItem *par = model->itemFromIndex(index)->parent();
			int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
			int r2 = model->itemFromIndex(index)->parent()->rowCount();

			set_child_item(name_mpo,user_data_mpo,par,r1);
			
			set_child_item("Добавить Средства",QString("pmposmi_%1").arg(list.value(1)),par,r2);
		
			}
		}
		else if(list.value(0)=="ppers"){
			
			Add_elements_dialog *add_element= new Add_elements_dialog(23,list.value(1).toInt());
			add_element->setModal(true);
		 	int result=add_element->exec();
			//вернул id 
			if (result==0)return;
			
			QSqlQuery query_pe;
			query_pe.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(result));
			while (query_pe.next())
			{
			QString name_pers=query_pe.value(1).toString();
			QString user_data_pers=QString("dpers_%1_%2").arg(QString::number(result)).arg(query_pe.value(0).toInt());

			QStandardItem *par = model->itemFromIndex(index)->parent();
			int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
			int r2 = model->itemFromIndex(index)->parent()->rowCount();

			set_child_item(name_pers,user_data_pers,par,r1);
			
			set_child_item("Добавить Персоналии",QString("ppers_%1").arg(list.value(1)),par,r2);
		
			}
        }
        else if(list.value(0)=="pperssmi"){

            Add_elements_dialog *add_element= new Add_elements_dialog(26,list.value(1).toInt());
            add_element->setModal(true);
            int result=add_element->exec();
            //вернул id
            if (result==0)return;

            QSqlQuery query_pers_smi;
            query_pers_smi.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(result));
            while (query_pers_smi.next())
            {
            QString name_pers_smi=query_pers_smi.value(1).toString();
            QString user_data_pers_smi=QString("dperssmi_%1_%2").arg(QString::number(result)).arg(query_pers_smi.value(0).toInt());

            QStandardItem *par = model->itemFromIndex(index)->parent();
            int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
            int r2 = model->itemFromIndex(index)->parent()->rowCount();

            set_child_item(name_pers_smi,user_data_pers_smi,par,r1);

            set_child_item("Добавить персоналии",QString("pperssmi_%1").arg(list.value(1)),par,r2);

            }
        }

        // ======================= для ВФ персоналии ===============================================================
		else if(list.value(0)=="ppersls"){
			
            Add_elements_dialog *add_element= new Add_elements_dialog(25,list.value(1).toInt());
			add_element->setModal(true);
            int result=add_element->exec();
			//вернул id 
			if (result==0)return;
			
			QSqlQuery query_pe;
			query_pe.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(result));
			while (query_pe.next())
			{
			QString name_pers=query_pe.value(1).toString();
			QString user_data_pers=QString("dpersls_%1_%2").arg(QString::number(result)).arg(query_pe.value(0).toInt());

			QStandardItem *par = model->itemFromIndex(index)->parent();
			int r1 = model->itemFromIndex(index)->parent()->rowCount()-1;
			int r2 = model->itemFromIndex(index)->parent()->rowCount();

			set_child_item(name_pers,user_data_pers,par,r1);
			
            set_child_item("Добавить Персоналии",QString("ppersls_%1").arg(list.value(1)),par,r2);
		
			}
		}

		// ======================= национальность ======================================
		else if(list.value(0)=="nations"){
			Add_elements_dialog *add_element= new Add_elements_dialog(4,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(700,350);
			add_element->exec();
		}
		else if(list.value(0)=="nationss"){
			Add_elements_dialog *add_element= new Add_elements_dialog(10,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(700,350);
			add_element->exec();
		}
		else if(list.value(0)=="nat"){
			Add_elements_dialog *add_element= new Add_elements_dialog(17,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(700,350);
			add_element->exec();
		}
		// ======================= религия ======================================
		else if(list.value(0)=="confess"){
			Add_elements_dialog *add_element= new Add_elements_dialog(6,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(500,350);
			add_element->exec();
		}
		else if(list.value(0)=="confesss"){
			Add_elements_dialog *add_element= new Add_elements_dialog(11,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(500,350);
			add_element->exec();
		}
		else if(list.value(0)=="conf"){
			Add_elements_dialog *add_element= new Add_elements_dialog(18,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(500,350);
			add_element->exec();
		}// ======================= профессиональный состав ======================================
		else if(list.value(0)=="profess"){
			Add_elements_dialog *add_element= new Add_elements_dialog(7,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(500,350);
			add_element->exec();
		}
		else if(list.value(0)=="prof"){
			Add_elements_dialog *add_element= new Add_elements_dialog(19,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(500,350);
			add_element->exec();
		}// ======================= возрастной состав ======================================
		else if(list.value(0)=="agess"){
			Add_elements_dialog *add_element= new Add_elements_dialog(8,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(500,350);
			add_element->exec();
		}//===================== для организаций =============================================
		else if(list.value(0)=="ag"){
			Add_elements_dialog *add_element= new Add_elements_dialog(20,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(500,350);
			add_element->exec();
		}// ======================= половой состав ======================================
		else if(list.value(0)=="sexss"){
			Add_elements_dialog *add_element= new Add_elements_dialog(9,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(600,100);
			add_element->exec();
		}
			else if(list.value(0)=="se"){
			Add_elements_dialog *add_element= new Add_elements_dialog(21,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(600,100);
			add_element->exec();
		}// ======================= Штатка =================================================
		else if(list.value(0)=="rankss"){
			Add_elements_dialog *add_element= new Add_elements_dialog(12,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(600,100);
			add_element->exec();
		}//=================== для организаций ===========================================
		else if(list.value(0)=="ran"){
			Add_elements_dialog *add_element= new Add_elements_dialog(22,list.value(1).toInt());
			add_element->setModal(true);
			add_element->setFixedSize(600,100);
			add_element->exec();
		}
		else if(list.value(0)=="dpers"){
			Add_elements_dialog *add_element= new Add_elements_dialog(24,list.value(1).toInt());
			add_element->setModal(true);
			connect(add_element->deleteButton,SIGNAL(clicked()),this,SLOT(delete_pers()));
			add_element->exec();

		}// =============== для формирований по персоналу ================================
        else if(list.value(0)=="dpersls"){
			Add_elements_dialog *add_element= new Add_elements_dialog(24,list.value(1).toInt());
			add_element->setModal(true);
			connect(add_element->deleteButton,SIGNAL(clicked()),this,SLOT(delete_pers()));
			add_element->exec();
		}
         // =============== для smi по персоналу ================================
        else if(list.value(0)=="dperssmi"){
        Add_elements_dialog *add_element= new Add_elements_dialog(24,list.value(1).toInt());
        add_element->setModal(true);
        connect(add_element->deleteButton,SIGNAL(clicked()),this,SLOT(delete_pers()));
        add_element->exec();
         }
		//============== выбор для заполнения таблицы ====================================
		else if(list.value(0)=="region"){
			region_click(list.value(1).toInt());
		}
		else if(list.value(0)=="reg"){
			region_click(list.value(1).toInt());
		} // ================= в таблицу данные о СМИ =======================================
        else if(list.value(0)=="dsmi"){
			smi_click(list.value(2).toInt());
		} // ================= в таблицу данные о ВФ =======================
		else if(list.value(0)=="dls"){
			ls_click(list.value(1).toInt());
		} // ================= в таблицу данные о Организациях =======================
		else if(list.value(0)=="dgr"){
			gr_click(list.value(1).toInt());
		}	// ================= в таблицу данные о Условиях =======================
		else if(list.value(0)=="dsc"){
			sc_click(list.value(1).toInt());
		}//======================= в таблицу воинские формирования (подчиненные)======
		else if(list.value(0)=="lss"){
			ls_click(list.value(1).toInt());
		}
		else if(list.value(0)=="chls"){
			ls_click(list.value(1).toInt());
		} // ================= в таблицу данные о СРЕДСТВАХ =======================
		else if(list.value(0)=="dmpo" || list.value(0)=="dmpos" || list.value(0)=="dmposmi" ){
			mpo_click(list.value(1).toInt());
	   	}
	}
}
//========================== ветка для воинских формирований ===========================================
void Objectmanager::child_ls_objects(QStandardItem *parent_item,int parent_ls)
{
	QSqlQuery query;
	int row=0;
	query.exec(QString("SELECT id_ls, name_ls, parent_ls,enimy_ls FROM ls WHERE parent_ls=%1 ORDER BY name_ls").arg(parent_ls));

	if (query.size() == 0){
		
	}
	else{
		while (query.next()){
			int id_object=query.value(0).toInt();
			if (id_object == parent_ls)continue;
			float mps_for_ls = calc_mps.calculating(id_object);
			bool enemy_ls = query.value(3).toBool();

			if (enemy_ls == true) {
			QBrush b(Qt::blue);
			child_ls_objects(set_child_item(query.value(1).toString(),
												"lss_" + QString::number(id_object) + "_" + QString::number(parent_ls) + "_" + QString::number(mps_for_ls) + "_" + QString("%1").arg(calc_mps.isEnemy(id_object)), 
												parent_item,
                                                row,iconsList.at(calc_mps_for_ls(mps_for_ls)),b),id_object);}   //b
			else {
			QBrush b(Qt::red);
			child_ls_objects(set_child_item(query.value(1).toString(),
												"lss_" + QString::number(id_object) + "_" + QString::number(parent_ls) + "_" + QString::number(mps_for_ls) + "_" + QString("%1").arg(calc_mps.isEnemy(id_object)), 
												parent_item,
                                                row,iconsList.at(calc_mps_for_ls(mps_for_ls)),b),id_object);} //b
			row++;
		}
	}
	set_child_item("Добавить подчиненные ВФ",QString("plss_%1").arg(parent_ls),parent_item,row);
	add_ls_components(parent_item,parent_ls,row);
	query.clear();
}
void Objectmanager::child_groups_objects(QStandardItem *parent_item,int gr)
{
	QSqlQuery query;
	int row=0;
	query.exec(QString("SELECT id_groups, name_groups FROM groups WHERE id_groups=%1 ORDER BY name_groups").arg(gr));

	if (query.size() == 0){
		
	}
	else{
		while (query.next()){
			int id_object=query.value(0).toInt();
			if (id_object == gr)continue;
			
			child_groups_objects(set_child_item(query.value(1).toString(),
												"dgr_" + QString::number(id_object) + "_" + QString::number(gr), 
												parent_item,
												row),id_object);}
			row++;
	}
	add_groups_components(parent_item,gr,row);
	query.clear();
}
void Objectmanager::child_smi_objects(QStandardItem *parent_item,int smi)
{
	QSqlQuery query;
	int row=0;
	query.exec(QString("SELECT id_smi, name_smi FROM smi WHERE id_smi=%1 ORDER BY name_smi").arg(smi));

	if (query.size() == 0){
		
	}
	else{
		while (query.next()){
			int id_object=query.value(0).toInt();
			if (id_object == smi)continue;
			
			child_smi_objects(set_child_item(query.value(1).toString(),
												"dsm_" + QString::number(id_object) + "_" + QString::number(smi), 
												parent_item,
												row),id_object);}
			row++;
	}
	add_smi_components(parent_item,smi,row);
	query.clear();
}
void Objectmanager::add_ls_components(QStandardItem *parent_item,int id_parent_region,int start_row){
// ========================================= средства =================================================	
	QSqlQuery query;
	query.exec(QString("SELECT id_ls,id_mpo_pso FROM mpo_pso WHERE id_ls=%1").arg(id_parent_region));
	int row=start_row+1;
	int row_sw=0;				
	int g; 
	QSqlQuery query_count;
	query_count.exec(QString("SELECT count(id_mpo_pso) FROM mpo_pso WHERE id_ls=%1").arg(id_parent_region));
	while (query_count.next()){
		g = query_count.value(0).toInt();}
	
	QStandardItem *item=set_child_item("СРЕДСТВА ["  + QString::number(g) + "]","mpo",parent_item,row,"./icons/connect_saturn.png");
	//QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
	row++;
	
	if (query.size() != 0)
	{
		row_sw=0;
		while (query.next())
		{
			QSqlQuery query_smi;
			int id_mpo=query.value(1).toInt();
			int id_mpo_ls=query.value(2).toInt();
			query_smi.exec(QString("SELECT id_mpo_pso,name_mpo_pso FROM mpo_pso WHERE id_mpo_pso=%1").arg(query.value(1).toInt()));
			while (query_smi.next())
			{
				set_child_item(query_smi.value(1).toString(),QString("dmpo_%1").arg(id_mpo),item,row_sw);
			}
			row_sw++;
		}
	 
	}	
	set_child_item("Добавить Средства",QString("pmpo_%1").arg(id_parent_region),item,row_sw);
	query.clear();
// ========================================= персонал =================================================	
	QSqlQuery query_pers;
	query_pers.exec(QString("SELECT id_ls,id_persones FROM persones WHERE id_ls=%1").arg(id_parent_region));
	int row_p=0;				
	int f; 
	QSqlQuery query_count_p;
	query_count_p.exec(QString("SELECT count(id_persones) FROM persones WHERE id_ls=%1").arg(id_parent_region));
	while (query_count_p.next()){
		f = query_count_p.value(0).toInt();}
	
	item=set_child_item("ПЕРСОНАЛИИ ["  + QString::number(f) + "]","persls",parent_item,row,"./icons/connect_saturn.png");
	//QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
	row++;
	
	if (query_pers.size() != 0)
	{
		row_p=0;
		while (query_pers.next())
		{
			QSqlQuery query_p;
			int id_person=query_pers.value(1).toInt();
			query_p.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(query_pers.value(1).toInt()));
			while (query_p.next())
			{
				set_child_item(query_p.value(1).toString(),QString("dpersls_%1").arg(id_person),item,row_p);
			}
			row_p++;
		}
	 
	}	
	set_child_item("Добавить Персоналии",QString("ppersls_%1").arg(id_parent_region),item,row_p);
	query.clear();

//========================= Национальность =========================================================
	QFont font;
	font.setBold(true);
//	int row=start_row+1;
	item=set_child_item("Национальный состав",QString("nationss_%1").arg(id_parent_region),parent_item,row,font);
	row++;
//============================ Религия =============================================================
	item=set_child_item("Религиозный состав",QString("confesss_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
//============================ Штатка ==============================================================
	item=set_child_item("Штатно-должностной состав",QString("rankss_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
//==========================================================================================================================
	




}	

void Objectmanager::add_groups_components(QStandardItem *parent_item,int id_parent_region,int start_row)
{
// ========================================= средства =================================================	
	QSqlQuery query;
    query.exec(QString("SELECT id_groups,id_mpo_pso FROM mpo_pso WHERE id_groups=%1").arg(id_parent_region));
	int row=0;
	int row_sw=0;				
	int g; 
	QSqlQuery query_count;
	query_count.exec(QString("SELECT count(id_mpo_pso) FROM mpo_pso WHERE id_groups=%1").arg(id_parent_region));
	while (query_count.next()){
		g = query_count.value(0).toInt();}
	
	QStandardItem *item=set_child_item("СРЕДСТВА ["  + QString::number(g) + "]","mpo",parent_item,row,"./icons/connect_saturn.png");
	//QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
	row++;
	
	if (query.size() != 0)
	{
		row_sw=0;
		while (query.next())
		{
			QSqlQuery query_smi;
			int id_groups=query.value(1).toInt();
			query_smi.exec(QString("SELECT id_mpo_pso,name_mpo_pso FROM mpo_pso WHERE id_mpo_pso=%1").arg(query.value(1).toInt()));
			while (query_smi.next())
			{
				set_child_item(query_smi.value(1).toString(),QString("dmpos_%1").arg(id_groups),item,row_sw);
			}
			row_sw++;
		}
	 
	}	
	set_child_item("Добавить Средства",QString("pmpos_%1").arg(id_parent_region),item,row_sw);
	query.clear();
// ========================================= персонал =================================================	
	QSqlQuery query_pers;
	query_pers.exec(QString("SELECT id_groups,id_persones FROM persones WHERE id_groups=%1").arg(id_parent_region));
	int row_p=0;				
	int f; 
	QSqlQuery query_count_p;
	query_count_p.exec(QString("SELECT count(id_persones) FROM persones WHERE id_groups=%1").arg(id_parent_region));
	while (query_count_p.next()){
		f = query_count_p.value(0).toInt();}
	
	item=set_child_item("ПЕРСОНАЛИИ ["  + QString::number(f) + "]","pers",parent_item,row,"./icons/connect_saturn.png");
	//QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
	row++;
	
	if (query_pers.size() != 0)
	{
		row_p=0;
		while (query_pers.next())
		{
			QSqlQuery query_p;
			int id_groups=query_pers.value(1).toInt();
			query_p.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(query_pers.value(1).toInt()));
			while (query_p.next())
			{
				set_child_item(query_p.value(1).toString(),QString("dpers_%1").arg(id_groups),item,row_p);
			}
			row_p++;
		}
	 
	}	
	set_child_item("Добавить Персоналии",QString("ppers_%1").arg(id_parent_region),item,row_p);
	query.clear();

//========================= Национальность =========================================================
	QFont font;
	font.setBold(true);
	item=set_child_item("Национальный состав",QString("nat_%1").arg(id_parent_region),parent_item,row,font);
	row++;
//============================ Религия =============================================================
	item=set_child_item("Религиозный состав",QString("conf_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
//============================ Профессиональный состав =============================================================

	item=set_child_item("Профессиональный состав",QString("prof_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
//============================ Возрастной состав =============================================================
	item=set_child_item("Возрастной состав",QString("ag_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
//============================ Половой состав =============================================================
	item=set_child_item("Половой состав",QString("se_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
//============================ Штатка ==============================================================
	item=set_child_item("Штатно-должностной состав",QString("ran_%1").arg(id_parent_region),parent_item,row,font);
	row++;	
//==========================================================================================================================
}


void Objectmanager::add_smi_components(QStandardItem *parent_item,int id_parent_region,int start_row)
{
// ========================================= средства =================================================	
	QSqlQuery query;
	query.exec(QString("SELECT id_smi,id_mpo_pso FROM mpo_pso WHERE id_smi=%1").arg(id_parent_region));
	int row=0;
	int row_sw=0;				
	int g; 
	QSqlQuery query_count;
	query_count.exec(QString("SELECT count(id_mpo_pso) FROM mpo_pso WHERE id_smi=%1").arg(id_parent_region));
	while (query_count.next()){
		g = query_count.value(0).toInt();}
	
	QStandardItem *item=set_child_item("СРЕДСТВА ["  + QString::number(g) + "]","mpo",parent_item,row,"./icons/connect_saturn.png");
	//QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
	row++;
	
	if (query.size() != 0)
	{
		row_sw=0;
		while (query.next())
		{
			QSqlQuery query_smi;
			int id_smi=query.value(1).toInt();
			query_smi.exec(QString("SELECT id_mpo_pso,name_mpo_pso FROM mpo_pso WHERE id_mpo_pso=%1").arg(query.value(1).toInt()));
			while (query_smi.next())
			{
				set_child_item(query_smi.value(1).toString(),QString("dmposmi_%1").arg(id_smi),item,row_sw);
			}
			row_sw++;
		}
	 
	}	
	set_child_item("Добавить Средства",QString("pmposmi_%1").arg(id_parent_region),item,row_sw);
	query.clear();
 // ========================================= персонал =================================================
        QSqlQuery query_pers;
        query_pers.exec(QString("SELECT id_smi,id_persones FROM persones WHERE id_smi=%1").arg(id_parent_region));
        int row_p=0;
        int t;
        QSqlQuery query_count_p;
        query_count_p.exec(QString("SELECT count(id_persones) FROM persones WHERE id_smi=%1").arg(id_parent_region));
        while (query_count_p.next()){
            t = query_count_p.value(0).toInt();}

        item=set_child_item("ПЕРСОНАЛИИ ["  + QString::number(t) + "]","perssmi",parent_item,row,"./icons/connect_saturn.png");
        //QStandardItem *item=set_child_item("СМИ","smi",parent_item,row,"./icons/printer.png");
        row++;

        if (query_pers.size() != 0)
        {
            row_p=0;
            while (query_pers.next())
            {
                QSqlQuery query_p;
                int id_smi=query_pers.value(1).toInt();
                query_p.exec(QString("SELECT id_persones,name_persones FROM persones WHERE id_persones=%1").arg(query_pers.value(1).toInt()));
                while (query_p.next())
                {
                    set_child_item(query_p.value(1).toString(),QString("dperssmi_%1").arg(id_smi),item,row_p);
                }
                row_p++;
            }

        }
        set_child_item("Добавить Персоналии",QString("pperssmi_%1").arg(id_parent_region),item,row_p);
        query.clear();
}

QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row)
	{
		QStandardItem *item = new QStandardItem(item_text.toStdString().c_str());
		item->setData(user_data,Qt::UserRole);
		parent_item->setChild(row,item);
		return item;
	}
QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QBrush b)
	{
		QStandardItem *item = new QStandardItem(item_text.toStdString().c_str());
		item->setForeground(b);
		item->setData(user_data,Qt::UserRole);
		parent_item->setChild(row,item);
		return item;
	}
QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QFont font)
	{
		QStandardItem *item = new QStandardItem(item_text.toStdString().c_str());
		item->setFont(font);
		item->setData(user_data,Qt::UserRole);
		parent_item->setChild(row,item);
		return item;
	}
QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path)	{
		QStandardItem *item = new QStandardItem(QIcon(icon_path),item_text.toStdString().c_str());
		item->setData(user_data,Qt::UserRole);
		parent_item->setChild(row,item);
		return item;
	}
QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path,QFont font)	{
		QStandardItem *item = new QStandardItem(QIcon(icon_path),item_text.toStdString().c_str());
		item->setFont(font);
		item->setData(user_data,Qt::UserRole);
		parent_item->setChild(row,item);
		return item;
	}
QStandardItem * Objectmanager::set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path,QBrush b)	{
        QStandardItem *item = new QStandardItem(QIcon(icon_path),item_text.toStdString().c_str());
        item->setForeground(b);
        item->setData(user_data,Qt::UserRole);
        parent_item->setChild(row,item);
        return item;
    }
//============= функции заполнения свойств в tableview ==================================
void Objectmanager::region_click(int id_region){
	QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);
	QString db_name=model_region->database().databaseName();
	QStringList tables=model_region->database().tables();
	const QString tableName="region";
	model_region->setTable(tableName);
	model_region->setFilter(QString("id_region=%1").arg(id_region));
	
  
 bool is= model_region->select();
	QString str=model_region->lastError().text();
	model_region->setEditStrategy(QSqlTableModel::OnFieldChange);

    model_region->setHeaderData(3, Qt::Horizontal,"Наименование региона");model_region->setHeaderData(5, Qt::Horizontal, "Описание региона");
	model_region->setHeaderData(4, Qt::Horizontal, "Тип региона");model_region->setHeaderData(6, Qt::Horizontal, "Численность населения");
	model_region->setHeaderData(7, Qt::Horizontal, "Плотность населения");model_region->setHeaderData(8, Qt::Horizontal, "Уровень имиграции");
	model_region->setHeaderData(9, Qt::Horizontal, "Уровень эммиграции");model_region->setHeaderData(10, Qt::Horizontal, "Уровень рождаемости");
	model_region->setHeaderData(11, Qt::Horizontal, "Уровень смертности");model_region->setHeaderData(12, Qt::Horizontal, "Естественный прирост населения");
	model_region->setHeaderData(13, Qt::Horizontal, "Уровень бедности(спф1)");	
	model_region->setHeaderData(14, Qt::Horizontal, "Pспф1");model_region->setHeaderData(15, Qt::Horizontal, "Уровень цен(спф2)");
	model_region->setHeaderData(16, Qt::Horizontal, "Pспф2");model_region->setHeaderData(17, Qt::Horizontal, "Уровень образования(спф3)");
	model_region->setHeaderData(18, Qt::Horizontal, "Pспф3");model_region->setHeaderData(19, Qt::Horizontal, "Степень доверия населения(спф4)");
	model_region->setHeaderData(20, Qt::Horizontal, "Pспф4");model_region->setHeaderData(21, Qt::Horizontal, "Степень поддержки населением(спф5)");
	model_region->setHeaderData(22, Qt::Horizontal, "Pспф5");model_region->setHeaderData(23, Qt::Horizontal, "Поддержка гос.структур(спф6)");
	model_region->setHeaderData(24, Qt::Horizontal, "Pспф6");model_region->setHeaderData(25, Qt::Horizontal, "Поддержка организациями ВС(спф7)");
	model_region->setHeaderData(26, Qt::Horizontal, "Pспф7");model_region->setHeaderData(27, Qt::Horizontal, "Влияние оппозиционных организаций(спф8)");
	model_region->setHeaderData(28, Qt::Horizontal, "Pспф8");model_region->setHeaderData(29, Qt::Horizontal, "Поддержка авторитетных деятелей(спф9)");
	model_region->setHeaderData(30, Qt::Horizontal, "Pспф9");model_region->setHeaderData(31, Qt::Horizontal, "Уровень безработицы(спф10)");
	model_region->setHeaderData(32, Qt::Horizontal, "Pспф10");model_region->setHeaderData(33, Qt::Horizontal, "Миграция населения (спф11)");
	model_region->setHeaderData(34, Qt::Horizontal, "Pспф11");model_region->setHeaderData(35, Qt::Horizontal, "Демография(спф12)");
	model_region->setHeaderData(36, Qt::Horizontal, "Pспф12");model_region->setHeaderData(37, Qt::Horizontal, "Уровень информатизации(спф13)");
	model_region->setHeaderData(38, Qt::Horizontal, "Pспф13");model_region->setHeaderData(39, Qt::Horizontal, "Направленность информации(спф14)");
	model_region->setHeaderData(40, Qt::Horizontal, "Pспф14");model_region->setHeaderData(41, Qt::Horizontal, "Защищенность инфроструктуры(спф15)");
	model_region->setHeaderData(42, Qt::Horizontal, "Pспф15");model_region->setHeaderData(43, Qt::Horizontal, "Неправительственные организации(спф16)");
	model_region->setHeaderData(44, Qt::Horizontal, "Pспф16");model_region->setHeaderData(45, Qt::Horizontal, "Уровень патриотизма(спф17)");
	model_region->setHeaderData(46, Qt::Horizontal, "Pспф17");model_region->setHeaderData(47, Qt::Horizontal, "Уровень преступности(кф1)");
	model_region->setHeaderData(48, Qt::Horizontal, "Pкф1");model_region->setHeaderData(49, Qt::Horizontal, "Корупция(кф2)");
	model_region->setHeaderData(50, Qt::Horizontal, "Pкф2");model_region->setHeaderData(51, Qt::Horizontal, "Теневой сектор(кф3)");
	model_region->setHeaderData(52, Qt::Horizontal, "Pкф3");model_region->setHeaderData(53, Qt::Horizontal, "Влияние некоренного населения(кф4)");
	model_region->setHeaderData(54, Qt::Horizontal, "Pкф4");model_region->setHeaderData(55, Qt::Horizontal, "Экстремизм,НВФ(кф5)");
	model_region->setHeaderData(56, Qt::Horizontal, "Pкф5");model_region->setHeaderData(57, Qt::Horizontal, "Исправительные учереждения(кф6)");
	model_region->setHeaderData(58, Qt::Horizontal, "Pкф6");model_region->setHeaderData(59, Qt::Horizontal, "Протестная активность(кф7)");
	model_region->setHeaderData(60, Qt::Horizontal, "Pкф7");model_region->setHeaderData(61, Qt::Horizontal, "Организованная преступность(кф8)");
	model_region->setHeaderData(62, Qt::Horizontal, "Pкф8");model_region->setHeaderData(63, Qt::Horizontal, "Оружие,наркотрафик(кф9)");
	model_region->setHeaderData(64, Qt::Horizontal, "Pкф9");model_region->setHeaderData(65, Qt::Horizontal, "Межнациональные конфликты(кф10)");
	model_region->setHeaderData(66, Qt::Horizontal, "Pкф10");model_region->setHeaderData(67, Qt::Horizontal, "Поддержка религиозных объединений(рф1)");
	model_region->setHeaderData(68, Qt::Horizontal, "Pрф1");model_region->setHeaderData(69, Qt::Horizontal, "Культовые сооружения(рф2)");
	model_region->setHeaderData(70, Qt::Horizontal, "Pрф2");model_region->setHeaderData(71, Qt::Horizontal, "Авторитет религиозных лидеров(рф3)");
	model_region->setHeaderData(72, Qt::Horizontal, "Pрф3");model_region->setHeaderData(73, Qt::Horizontal, "Поддержка религиозных лидеров(рф4)");
	model_region->setHeaderData(74, Qt::Horizontal, "Pрф4");model_region->setHeaderData(75, Qt::Horizontal, "Религиозность противника(рф5)");
	model_region->setHeaderData(76, Qt::Horizontal, "Pрф5");model_region->setHeaderData(77, Qt::Horizontal, "Религиозность своих войск(рф6)");
	model_region->setHeaderData(78, Qt::Horizontal, "Pрф6");model_region->setHeaderData(79, Qt::Horizontal, "Групповой коэффициент(спф)");
    model_region->setHeaderData(80, Qt::Horizontal, "Групповой коэффициент(кф)");model_region->setHeaderData(81, Qt::Horizontal, "Групповой коэффициент(рф)");
	model_region->setHeaderData(82, Qt::Horizontal, "Код значка региона");

	UI->property_object->setModel(model_region);

	UI->property_object->hideColumn(0);
	UI->property_object->hideColumn(1);
	UI->property_object->hideColumn(2);
	UI->property_object->setColumnWidth(3,150);UI->property_object->setColumnWidth(7,150);UI->property_object->setColumnWidth(11,150);
	UI->property_object->setColumnWidth(4,150);UI->property_object->setColumnWidth(8,150);UI->property_object->setColumnWidth(12,200);
	UI->property_object->setColumnWidth(5,150);UI->property_object->setColumnWidth(9,150);UI->property_object->setColumnWidth(13,155);
	UI->property_object->setColumnWidth(6,150);UI->property_object->setColumnWidth(10,150);UI->property_object->setColumnWidth(14,50);
	UI->property_object->setColumnWidth(15,130);UI->property_object->setColumnWidth(16,50);UI->property_object->setColumnWidth(17,170);
	UI->property_object->setColumnWidth(18,50);UI->property_object->setColumnWidth(19,200);UI->property_object->setColumnWidth(20,50);
	UI->property_object->setColumnWidth(21,225);UI->property_object->setColumnWidth(22,50);UI->property_object->setColumnWidth(23,210);
	UI->property_object->setColumnWidth(24,50);UI->property_object->setColumnWidth(25,215);UI->property_object->setColumnWidth(26,50);
	UI->property_object->setColumnWidth(27,265);UI->property_object->setColumnWidth(28,50);UI->property_object->setColumnWidth(29,245);
	UI->property_object->setColumnWidth(30,50);UI->property_object->setColumnWidth(31,180);UI->property_object->setColumnWidth(32,50);
	UI->property_object->setColumnWidth(33,180);UI->property_object->setColumnWidth(34,50);
	UI->property_object->setColumnWidth(35,130);UI->property_object->setColumnWidth(36,50);UI->property_object->setColumnWidth(49,115);
	UI->property_object->setColumnWidth(37,200);UI->property_object->setColumnWidth(38,50);UI->property_object->setColumnWidth(51,145);
	UI->property_object->setColumnWidth(39,220);UI->property_object->setColumnWidth(40,50);UI->property_object->setColumnWidth(53,240);
	UI->property_object->setColumnWidth(41,235);UI->property_object->setColumnWidth(42,50);UI->property_object->setColumnWidth(52,50);
	UI->property_object->setColumnWidth(43,250);UI->property_object->setColumnWidth(44,50);UI->property_object->setColumnWidth(54,50);
	UI->property_object->setColumnWidth(45,175);UI->property_object->setColumnWidth(46,50);UI->property_object->setColumnWidth(48,50);
	UI->property_object->setColumnWidth(47,175);UI->property_object->setColumnWidth(50,50);
	UI->property_object->setColumnWidth(55,150);UI->property_object->setColumnWidth(56,50);
	UI->property_object->setColumnWidth(57,220);UI->property_object->setColumnWidth(58,50);UI->property_object->setColumnWidth(65,225);
	UI->property_object->setColumnWidth(59,180);UI->property_object->setColumnWidth(60,50);UI->property_object->setColumnWidth(66,50);
	UI->property_object->setColumnWidth(61,220);UI->property_object->setColumnWidth(62,50);
	UI->property_object->setColumnWidth(63,170);UI->property_object->setColumnWidth(64,50);
	UI->property_object->setColumnWidth(67,250);UI->property_object->setColumnWidth(68,50);
	UI->property_object->setColumnWidth(69,170);UI->property_object->setColumnWidth(70,50);
	UI->property_object->setColumnWidth(71,235);UI->property_object->setColumnWidth(72,50);
	UI->property_object->setColumnWidth(73,235);UI->property_object->setColumnWidth(74,50);
	UI->property_object->setColumnWidth(75,210);UI->property_object->setColumnWidth(76,50);
	UI->property_object->setColumnWidth(77,210);UI->property_object->setColumnWidth(78,50);
	UI->property_object->setColumnWidth(79,140);
	UI->property_object->setColumnWidth(80,180);
	UI->property_object->setColumnWidth(81,180);
	UI->property_object->setColumnWidth(82,140);
	UI->property_object->setShowGrid(true);

	UI->property_object->setItemDelegateForColumn(13,delegat); UI->property_object->setItemDelegateForColumn(14,delegat);
	UI->property_object->setItemDelegateForColumn(15,delegat); UI->property_object->setItemDelegateForColumn(16,delegat); 
	UI->property_object->setItemDelegateForColumn(17,delegat); UI->property_object->setItemDelegateForColumn(18,delegat);
	UI->property_object->setItemDelegateForColumn(19,delegat); UI->property_object->setItemDelegateForColumn(20,delegat); 
	UI->property_object->setItemDelegateForColumn(21,delegat); UI->property_object->setItemDelegateForColumn(22,delegat);
	UI->property_object->setItemDelegateForColumn(23,delegat); UI->property_object->setItemDelegateForColumn(24,delegat); 
	UI->property_object->setItemDelegateForColumn(25,delegat); UI->property_object->setItemDelegateForColumn(26,delegat);
	UI->property_object->setItemDelegateForColumn(27,delegat); UI->property_object->setItemDelegateForColumn(28,delegat); 
	UI->property_object->setItemDelegateForColumn(29,delegat); UI->property_object->setItemDelegateForColumn(30,delegat);
	UI->property_object->setItemDelegateForColumn(31,delegat); UI->property_object->setItemDelegateForColumn(32,delegat); 
	UI->property_object->setItemDelegateForColumn(33,delegat); UI->property_object->setItemDelegateForColumn(34,delegat);
	UI->property_object->setItemDelegateForColumn(35,delegat); UI->property_object->setItemDelegateForColumn(36,delegat); 
	UI->property_object->setItemDelegateForColumn(37,delegat); UI->property_object->setItemDelegateForColumn(38,delegat);
	UI->property_object->setItemDelegateForColumn(39,delegat); UI->property_object->setItemDelegateForColumn(40,delegat); 
	UI->property_object->setItemDelegateForColumn(41,delegat); UI->property_object->setItemDelegateForColumn(42,delegat);
	UI->property_object->setItemDelegateForColumn(43,delegat); UI->property_object->setItemDelegateForColumn(44,delegat); 
	UI->property_object->setItemDelegateForColumn(45,delegat); UI->property_object->setItemDelegateForColumn(46,delegat);
	UI->property_object->setItemDelegateForColumn(47,delegat); UI->property_object->setItemDelegateForColumn(48,delegat); 
	UI->property_object->setItemDelegateForColumn(49,delegat); UI->property_object->setItemDelegateForColumn(50,delegat);
	UI->property_object->setItemDelegateForColumn(51,delegat); UI->property_object->setItemDelegateForColumn(52,delegat); 
	UI->property_object->setItemDelegateForColumn(53,delegat); UI->property_object->setItemDelegateForColumn(54,delegat);
	UI->property_object->setItemDelegateForColumn(55,delegat); UI->property_object->setItemDelegateForColumn(56,delegat); 
	UI->property_object->setItemDelegateForColumn(57,delegat); UI->property_object->setItemDelegateForColumn(58,delegat);
	UI->property_object->setItemDelegateForColumn(59,delegat); UI->property_object->setItemDelegateForColumn(60,delegat); 
	UI->property_object->setItemDelegateForColumn(61,delegat); UI->property_object->setItemDelegateForColumn(62,delegat);
	UI->property_object->setItemDelegateForColumn(63,delegat); UI->property_object->setItemDelegateForColumn(64,delegat);
	UI->property_object->setItemDelegateForColumn(65,delegat); UI->property_object->setItemDelegateForColumn(66,delegat);
	UI->property_object->setItemDelegateForColumn(67,delegat); UI->property_object->setItemDelegateForColumn(68,delegat); 
	UI->property_object->setItemDelegateForColumn(69,delegat); UI->property_object->setItemDelegateForColumn(70,delegat);
	UI->property_object->setItemDelegateForColumn(71,delegat); UI->property_object->setItemDelegateForColumn(72,delegat); 
	UI->property_object->setItemDelegateForColumn(73,delegat); UI->property_object->setItemDelegateForColumn(74,delegat);
	UI->property_object->setItemDelegateForColumn(75,delegat); UI->property_object->setItemDelegateForColumn(76,delegat);
	UI->property_object->setItemDelegateForColumn(77,delegat); UI->property_object->setItemDelegateForColumn(78,delegat); 
	UI->property_object->setItemDelegateForColumn(79,delegat); UI->property_object->setItemDelegateForColumn(80,delegat);
	UI->property_object->setItemDelegateForColumn(81,delegat);

//	UI->property_object->setItemDelegate(new QSqlRelationalDelegate(UI->property_object));
//=============================================================================================
}
//=======================================================================================
void Objectmanager::smi_click(int id_smi){

	QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);
	if(id_smi<1){
		UI->property_object->setModel(model_region);
		return;
	}
	const QString tableName="smi";
	model_region->setTable(tableName);
	model_region->setFilter(QString("id_smi=%1").arg(id_smi));
    UI->property_object->verticalHeader()->setVisible(true);
/*
QStandardItemModel *model = new QStandardItemModel(3,3, UI->property_object);
UI->property_object->setModel(model);
 
for(int row=0; row!=model->rowCount(); ++row){
    for(int column=0; column!=model->columnCount(); ++column) {
        QStandardItem *newItem = new QStandardItem(tr("%1").arg((row+1)*(column+1)));
        model->setItem(row, column, newItem);
    }
}*/

	model_region->setRelation(1,QSqlRelation("type_smi","id_type_smi","nametype_smi"));
    model_region->setHeaderData(1,Qt::Horizontal,"Тип СМИ");
    model_region->setRelation(2,QSqlRelation("type_office_smi","id_type_office_smi","name_type_office_smi"));
    model_region->setHeaderData(2,Qt::Horizontal,"Представительство СМИ");
	model_region->setRelation(3,QSqlRelation("type_broadcast_smi","id_type_broadcast_smi","name_type_broadcast_smi"));
	model_region->setHeaderData(3,Qt::Horizontal,"Способ вещания СМИ");
	model_region->setRelation(4,QSqlRelation("position_smi","id_position_smi","name_position_smi"));
	model_region->setHeaderData(4,Qt::Horizontal,"Направленность СМИ");
	model_region->setRelation(5,QSqlRelation("level_smi","id_level_smi","name_level_smi"));
	model_region->setHeaderData(5,Qt::Horizontal,"Уровень вещания");
	model_region->setRelation(6,QSqlRelation("theme_smi","id_theme_smi","name_theme_smi"));
	model_region->setHeaderData(6,Qt::Horizontal,"Тематика СМИ");
	model_region->setHeaderData(7, Qt::Horizontal,"Наименование СМИ");
	model_region->setHeaderData(8, Qt::Horizontal,"Мощность сигнала");
	model_region->setHeaderData(9, Qt::Horizontal,"Сайт СМИ");
	model_region->setHeaderData(10, Qt::Horizontal,"Описание СМИ");
	model_region->setHeaderData(11, Qt::Horizontal,"Частоты вещания");
	model_region->setHeaderData(13, Qt::Horizontal,"Аудитория СМИ");
	model_region->setHeaderData(14, Qt::Horizontal,"Тираж СМИ");
//	model_region->setHeaderData(15, Qt::Horizontal,"Защищенность");
	
	bool is= model_region->select();

	model_region->setEditStrategy(QSqlTableModel::OnFieldChange);

	UI->property_object->clearSpans();
  //  UI->property_object->selectionModel();
    UI->property_object->setModel(model_region);

	UI->property_object->hideColumn(0);
	UI->property_object->hideColumn(12);
//	UI->property_object->hideColumn(15);

//	UI->property_object->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
//	UI->property_object->setEditTriggers(QAbstractItemView::NoEditTriggers);
//	UI->property_object->setAlternatingRowColors(true);

	UI->property_object->setColumnWidth(1,200);UI->property_object->setColumnWidth(2,200);UI->property_object->setColumnWidth(3,200);
	UI->property_object->setColumnWidth(4,200);UI->property_object->setColumnWidth(5,200);UI->property_object->setColumnWidth(6,200);
	UI->property_object->setColumnWidth(7,200);UI->property_object->setColumnWidth(8,200);UI->property_object->setColumnWidth(9,200);
	UI->property_object->setColumnWidth(10,200);UI->property_object->setColumnWidth(11,200);UI->property_object->setColumnWidth(13,200);
	UI->property_object->setColumnWidth(14,200);
	
	UI->property_object->setShowGrid(true);
	QSqlRelationalDelegate *delegat_smi=new QSqlRelationalDelegate(UI->property_object);
	UI->property_object->setItemDelegateForColumn(13,delegat_smi);UI->property_object->setItemDelegateForColumn(14,delegat_smi);

}
void Objectmanager::ls_click (int id_ls)
{
QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);

	const QString tableName="ls";
	model_region->setTable(tableName);
	model_region->setFilter(QString("id_ls=%1").arg(id_ls));
    UI->property_object->verticalHeader()->setVisible(true);
	
	model_region->setRelation(2,QSqlRelation("region","id_region","name_region"));
    model_region->setHeaderData(2,Qt::Horizontal,"Регион");
	model_region->setHeaderData(3,Qt::Horizontal,"Наименование");
	model_region->setHeaderData(4,Qt::Horizontal,"Враждебность подразделения");
	model_region->setHeaderData(5,Qt::Horizontal,"Подразделение МПО");
	model_region->setHeaderData(6,Qt::Horizontal,"Численность л/с");
	model_region->setHeaderData(7,Qt::Horizontal,"Численность л/с в БД");
	model_region->setHeaderData(8,Qt::Horizontal,"л/с по призыву");
	model_region->setHeaderData(9,Qt::Horizontal,"л/с по по контракту");
	model_region->setHeaderData(10,Qt::Horizontal,"Офицеры");
    model_region->setRelation(12,QSqlRelation("type_ls","id_type_ls","name_type_ls"));
    model_region->setHeaderData(12,Qt::Horizontal,"Тип подразделения");
	model_region->setHeaderData(13,Qt::Horizontal,"Сокращенное наименование подразделения");

	
		
	bool is= model_region->select();

	model_region->setEditStrategy(QSqlTableModel::OnFieldChange);

    UI->property_object->setModel(model_region);
	UI->property_object->sortByColumn(2,Qt::AscendingOrder);
	UI->property_object->hideColumn(0);
	UI->property_object->hideColumn(1);
	UI->property_object->hideColumn(11);

	UI->property_object->setColumnWidth(2,200);
	UI->property_object->setColumnWidth(3,200);//UI->property_object->setColumnWidth(3,200);
	UI->property_object->setColumnWidth(4,200);
	UI->property_object->setColumnWidth(5,200);
	UI->property_object->setColumnWidth(6,200);
	UI->property_object->setColumnWidth(7,200);
	UI->property_object->setColumnWidth(8,200);
	UI->property_object->setColumnWidth(9,200);
	UI->property_object->setColumnWidth(10,200);
	UI->property_object->setColumnWidth(12,200);
	UI->property_object->setColumnWidth(13,200);

	UI->property_object->setShowGrid(true);
	QSqlRelationalDelegate *delegat_ls=new QSqlRelationalDelegate(UI->property_object);
	UI->property_object->setItemDelegate(delegat_ls);
	UI->property_object->setItemDelegateForColumn(13,delegat_ls);

	QSqlQuery query;
		query.exec(QString("select id_coordinates,name_ls from ls where id_ls=%1").arg(id_ls));
		int id_coordinates;
		QString name_gr;
		while (query.next()) 
		{
			id_coordinates=query.value(0).toInt();
			name_gr = query.value(1).toString();
		}

	QSqlRelationalTableModel *model_coord = new QSqlRelationalTableModel(this);
	model_coord->setTable("coordinates");
	model_coord->setFilter(QString("id_coordinates=%1").arg(id_coordinates));
	is = model_coord->select();
	model_coord->setEditStrategy(QSqlTableModel::OnFieldChange);
	model_coord->setHeaderData(1, Qt::Horizontal,"Координаты X");
	model_coord->setHeaderData(2, Qt::Horizontal, "Координаты Y");
	UI->coord_object->setModel(model_coord);
//	UI->label_coord->setText("Координаты организации " + name_gr + ":");
	UI->coord_object->hideColumn(0);

}
void Objectmanager::gr_click (int id_gr)
{
QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);
	
	if(id_gr<1){
		UI->property_object->setModel(model_region);
		return;
	}
	//else UI->property_object->horizontalHeader()->setVisible(true);
	const QString tableName="groups";
	model_region->setTable(tableName);
	model_region->setFilter(QString("id_groups=%1").arg(id_gr));
	
	model_region->setRelation(1,QSqlRelation("sphere_groups","id_sphere_groups","name_sphere_groups"));
	model_region->setHeaderData(1,Qt::Horizontal,"Сфера деятельности");	
	model_region->setRelation(2,QSqlRelation("form_groups","id_form_groups","name_form_groups"));
	model_region->setHeaderData(2,Qt::Horizontal,"Форма организации");
	model_region->setRelation(3,QSqlRelation("trend_groups","id_trend_groups","name_trend_groups"));
	model_region->setHeaderData(3,Qt::Horizontal,"Направленность");	
	model_region->setHeaderData(4, Qt::Horizontal,"Наименование организации");
	model_region->setHeaderData(5, Qt::Horizontal,"Численность организации");
	model_region->setHeaderData(6, Qt::Horizontal,"Учредители организации");
	model_region->setHeaderData(7, Qt::Horizontal,"Руководство организации");
	model_region->setHeaderData(8, Qt::Horizontal,"Представительство организации");
	model_region->setHeaderData(9, Qt::Horizontal,"Описание организации");
	model_region->setHeaderData(10, Qt::Horizontal,"Оппозиционность организации");
	model_region->setHeaderData(14, Qt::Horizontal,"Информационные органы");
		
	bool is= model_region->select();

	model_region->setEditStrategy(QSqlTableModel::OnFieldChange);
	UI->property_object->clearSpans();
	UI->property_object->setModel(model_region);
	UI->property_object->hideColumn(0);
	UI->property_object->hideColumn(11);
	UI->property_object->hideColumn(12);
	UI->property_object->hideColumn(13);
	
	//UI->property_object->setColumnWidth(1,150);
	UI->property_object->setColumnWidth(1,200);UI->property_object->setColumnWidth(2,200);
	UI->property_object->setColumnWidth(3,200);UI->property_object->setColumnWidth(4,200);UI->property_object->setColumnWidth(5,200);
	UI->property_object->setColumnWidth(6,200);UI->property_object->setColumnWidth(7,200);UI->property_object->setColumnWidth(8,200);
	UI->property_object->setColumnWidth(9,200);UI->property_object->setColumnWidth(10,200);UI->property_object->setColumnWidth(14,200);
	
	UI->property_object->setShowGrid(true);
	QSqlRelationalDelegate *delegat_gr=new QSqlRelationalDelegate(UI->property_object);
	UI->property_object->setItemDelegate(delegat_gr);
	UI->property_object->setItemDelegateForColumn(13,delegat_gr);UI->property_object->setItemDelegateForColumn(14,delegat_gr);
	

		QSqlQuery query;
		query.exec(QString("select id_coordinates,name_groups from groups where id_groups=%1").arg(id_gr));
		int id_coordinates;
		QString name_gr;
		while (query.next()) 
		{
			id_coordinates=query.value(0).toInt();
			name_gr = query.value(1).toString();
		}

	QSqlRelationalTableModel *model_coord = new QSqlRelationalTableModel(this);
	model_coord->setTable("coordinates");
	model_coord->setFilter(QString("id_coordinates=%1").arg(id_coordinates));
	is = model_coord->select();
	model_coord->setEditStrategy(QSqlTableModel::OnFieldChange);
	model_coord->setHeaderData(1, Qt::Horizontal,"Координаты X");
	model_coord->setHeaderData(2, Qt::Horizontal, "Координаты Y");
	UI->coord_object->setModel(model_coord);
//	UI->label_coord->setText("Координаты организации " + name_gr + ":");
	UI->coord_object->hideColumn(0);
}
void Objectmanager::mpo_click (int id_mpo){
QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);
	
	if(id_mpo<1){
		UI->property_object->setModel(model_region);
		return;
	}
	//else UI->property_object->horizontalHeader()->setVisible(true);
	const QString tableName="mpo_pso";
	model_region->setTable(tableName);
	model_region->setFilter(QString("id_mpo_pso=%1").arg(id_mpo));
	
	model_region->setRelation(1,QSqlRelation("type_mpo_pso","id_type_mpo_pso","name_type_mpo_pso"));
	model_region->setHeaderData(1,Qt::Horizontal,"Тип средства");	
	model_region->setHeaderData(6, Qt::Horizontal,"Наименование средства");
	model_region->setHeaderData(7, Qt::Horizontal,"Количество средств");
	model_region->setHeaderData(8, Qt::Horizontal,"Описание средства");
			
	bool is= model_region->select();

	model_region->setEditStrategy(QSqlTableModel::OnFieldChange);
	UI->property_object->clearSpans();
	UI->property_object->setModel(model_region);
	UI->property_object->hideColumn(0);
	UI->property_object->hideColumn(2);
	UI->property_object->hideColumn(3);
	UI->property_object->hideColumn(4);
	UI->property_object->hideColumn(5);

	//UI->property_object->setColumnWidth(1,150);
	UI->property_object->setColumnWidth(1,200);UI->property_object->setColumnWidth(6,200);
	UI->property_object->setColumnWidth(7,200);UI->property_object->setColumnWidth(8,200);

	UI->property_object->setShowGrid(true);
	QSqlRelationalDelegate *delegat_mpo=new QSqlRelationalDelegate(UI->property_object);
	UI->property_object->setItemDelegate(delegat_mpo);

		QSqlQuery query;
		query.exec(QString("select id_coordinates,name_mpo_pso from mpo_pso where id_mpo_pso=%1").arg(id_mpo));
		int id_coordinates;
		QString name_mpo;
		while (query.next()) 
		{
			id_coordinates=query.value(0).toInt();
			name_mpo = query.value(1).toString();
		}

	QSqlRelationalTableModel *model_coord = new QSqlRelationalTableModel(this);
	model_coord->setTable("coordinates");
	model_coord->setFilter(QString("id_coordinates=%1").arg(id_coordinates));
	is = model_coord->select();
	model_coord->setEditStrategy(QSqlTableModel::OnFieldChange);
	model_coord->setHeaderData(1, Qt::Horizontal,"Координаты X");
	model_coord->setHeaderData(2, Qt::Horizontal, "Координаты Y");
	UI->coord_object->setModel(model_coord);
//	UI->label_coord->setText("Координаты организации " + name_gr + ":");
	UI->coord_object->hideColumn(0);
}

void Objectmanager::sc_click (int id_sc){
	QSqlRelationalTableModel *model_region = new QSqlRelationalTableModel(this);
	
	if(id_sc<1){
		UI->property_object->setModel(model_region);
		return;
	}
	//else UI->property_object->horizontalHeader()->setVisible(true);
	const QString tableName="special_conditions";
	model_region->setTable(tableName);
	model_region->setFilter(QString("id_special_conditions=%1").arg(id_sc));
	
	model_region->setRelation(2,QSqlRelation("type_special_conditions","id_type_special_conditions","name_type_special_conditions"));
	model_region->setHeaderData(2,Qt::Horizontal,"Тип особого условия");	
	model_region->setHeaderData(3, Qt::Horizontal,"Наименование особого условия");
	model_region->setHeaderData(4, Qt::Horizontal,"Описание особого условия");
	model_region->setHeaderData(5, Qt::Horizontal,"Семантика_1");
	model_region->setHeaderData(6, Qt::Horizontal,"Семантика_2");
			
	bool is= model_region->select();

	model_region->setEditStrategy(QSqlTableModel::OnFieldChange);
	UI->property_object->clearSpans();
	UI->property_object->setModel(model_region);
	UI->property_object->hideColumn(0);
	UI->property_object->hideColumn(1);
	

	//UI->property_object->setColumnWidth(1,150);
	UI->property_object->setColumnWidth(2,200);UI->property_object->setColumnWidth(3,200);
	UI->property_object->setColumnWidth(4,200);UI->property_object->setColumnWidth(5,200);
	UI->property_object->setColumnWidth(6,200);

	UI->property_object->setShowGrid(true);
	QSqlRelationalDelegate *delegat_sc=new QSqlRelationalDelegate(UI->property_object);
	UI->property_object->setItemDelegate(delegat_sc);

	QSqlRelationalTableModel *model_coord = new QSqlRelationalTableModel(this);
		QSqlQuery query;
		query.exec(QString("select id_coordinates from coord_spec_cond where id_special_conditions=%1 order by id_coordinates").arg(id_sc));
		QVector<int> id_coordinates;
		while (query.next()) {
			id_coordinates.append(query.value(0).toInt());
		}		
		
	
		QString str_filterQuery;
		for (int i=0; i<id_coordinates.size(); i++){	
			if(i!=0)
			{
				str_filterQuery +=" or ";
			}
			str_filterQuery += " id_coordinates=";
			str_filterQuery += QString::number(id_coordinates.at(i));
		}
		if (str_filterQuery == ""){
			return;
		}
		model_coord->setTable("coordinates");
		model_coord->setFilter(str_filterQuery);
		model_coord->setHeaderData(1, Qt::Horizontal,"Координаты X");
		model_coord->setHeaderData(2, Qt::Horizontal, "Координаты Y");
				
		is = model_coord->select();	
		model_coord->setEditStrategy(QSqlTableModel::OnFieldChange);
		UI->coord_object->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
		UI->coord_object->setModel(model_coord);
		UI->coord_object->hideColumn(0);
}

//==========================  удаление по нажатию контекстного меню ================================

void Objectmanager::delete_region(){
	QMessageBox msgBox;
	msgBox.setWindowTitle("Предупреждение");
	msgBox.setText("Вы действительно хотите удалить регион?"); 
	
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, "Да");
	msgBox.setButtonText(QMessageBox::No, "Нет");
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
	QSqlQuery query;

	QModelIndex index = UI->columnView->currentIndex();
	if(!index.data(Qt::UserRole).toBool()) return;
	QString id_region=index.data(Qt::UserRole).toString();
	QStringList list=id_region.split("_");
	
	QString str = QString("DELETE FROM region WHERE id_region = %1").arg(list.value(1));//.toInt());
	
	if(!query.exec(str)){
	QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление региона не выполнено"),QMessageBox::Ok );
	return;
	}
	UI->columnView->model()->removeRow(index.row(),index.parent()); 
	
	QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление региона выполнено"),QMessageBox::Ok );
	UI->property_object->setModel(0);

}
void Objectmanager::delete_smi(){
	QMessageBox msgBox;
	msgBox.setWindowTitle("Предупреждение");
	msgBox.setText("Вы действительно хотите удалить СМИ?"); 
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, "Да");
	msgBox.setButtonText(QMessageBox::No, "Нет");
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
	QSqlQuery query;

	QModelIndex index = UI->columnView->currentIndex();
	if(!index.data(Qt::UserRole).toBool()) return;
	QString id_smi_region=index.data(Qt::UserRole).toString();
	QStringList list=id_smi_region.split("_");
	
	QString str = QString("DELETE FROM smi_region WHERE id_smi_region = %1").arg(list.value(1));
	
	if(!query.exec(str)){
	QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление СМИ не выполнено"),QMessageBox::Ok );
	 return;
	}
	UI->columnView->model()->removeRow(index.row(),index.parent()); 
	QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление СМИ выполнено"),QMessageBox::Ok );
	
	UI->property_object->setModel(0);

}
void Objectmanager::delete_ls(){
	QMessageBox msgBox;
	msgBox.setWindowTitle("Предупреждение");
	msgBox.setText("Вы действительно хотите удалить воинское формирование?"); 
	
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, "Да");
	msgBox.setButtonText(QMessageBox::No, "Нет");
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
	QSqlQuery query;

	QModelIndex index = UI->columnView->currentIndex();
	if(!index.data(Qt::UserRole).toBool()) return;
	QString id_ls=index.data(Qt::UserRole).toString();
	QStringList list=id_ls.split("_");
	
	QString str = QString("DELETE FROM ls WHERE id_ls = %1").arg(list.value(1));
	
	if(!query.exec(str)){
	QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление ВФ не выполнено"),QMessageBox::Ok );
	 return;
	}
	UI->columnView->model()->removeRow(index.row(),index.parent()); 
	QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление ВФ выполнено"),QMessageBox::Ok );
	UI->property_object->setModel(0);
	UI->coord_object->setModel(0);
}
void Objectmanager::delete_groups(){
	QMessageBox msgBox;
	msgBox.setWindowTitle("Предупреждение");
	msgBox.setText("Вы действительно хотите удалить организацию?"); 
	
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, "Да");
	msgBox.setButtonText(QMessageBox::No, "Нет");
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
	QSqlQuery query;

	QModelIndex index = UI->columnView->currentIndex();
	if(!index.data(Qt::UserRole).toBool()) return;
	QString id_groups=index.data(Qt::UserRole).toString();
	QStringList list=id_groups.split("_");
	
	QString str = QString("DELETE FROM groups WHERE id_groups = %1").arg(list.value(1));
	
	if(!query.exec(str)){
	QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление организации не выполнено"),QMessageBox::Ok );
	 return;
	}
	UI->columnView->model()->removeRow(index.row(),index.parent()); 
	QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление организации выполнено"),QMessageBox::Ok );
	UI->property_object->setModel(0);
	UI->coord_object->setModel(0);
}
void Objectmanager::delete_mpo(){
	QMessageBox msgBox;
	msgBox.setWindowTitle("Предупреждение");
	msgBox.setText("Вы действительно хотите удалить средство?"); 
	
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, "Да");
	msgBox.setButtonText(QMessageBox::No, "Нет");
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
	QSqlQuery query;

	QModelIndex index = UI->columnView->currentIndex();
	if(!index.data(Qt::UserRole).toBool()) return;
	QString id_groups=index.data(Qt::UserRole).toString();
	QStringList list=id_groups.split("_");
	
	
	QString str1 = QString("DELETE FROM mpo_pso WHERE id_mpo_pso = %1").arg(list.value(1));
	if(!query.exec(str1)){
	QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление средства не выполнено"),QMessageBox::Ok );
	 return;
	}
	UI->columnView->model()->removeRow(index.row(),index.parent()); 
	QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление средства выполнено"),QMessageBox::Ok );
	UI->property_object->setModel(0);
	UI->coord_object->setModel(0);
}
void Objectmanager::delete_sc(){
	QMessageBox msgBox;
	msgBox.setWindowTitle("Предупреждение");
	msgBox.setText("Вы действительно хотите удалить особое условие?"); 
	
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, "Да");
	msgBox.setButtonText(QMessageBox::No, "Нет");
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
	QSqlQuery query;

	QModelIndex index = UI->columnView->currentIndex();
	if(!index.data(Qt::UserRole).toBool()) return;
	QString id_sc=index.data(Qt::UserRole).toString();
	QStringList list=id_sc.split("_");
	
	QString str = QString("DELETE FROM special_conditions WHERE id_special_conditions = %1").arg(list.value(1));
	
	if(!query.exec(str)){
	QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление особого условия не выполнено"),QMessageBox::Ok );
	 return;
	}
	UI->columnView->model()->removeRow(index.row(),index.parent()); 
	QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление особого условия выполнено"),QMessageBox::Ok );
	UI->property_object->setModel(0);
	UI->coord_object->setModel(0);
}

void Objectmanager::delete_pers(){
	QMessageBox msgBox;
	msgBox.setWindowTitle("Предупреждение");
	msgBox.setText("Вы действительно хотите удалить данные по персоналу?"); 
	
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, "Да");
	msgBox.setButtonText(QMessageBox::No, "Нет");
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
	QSqlQuery query;

	QModelIndex index = UI->columnView->currentIndex();
	if(!index.data(Qt::UserRole).toBool()) return;
	QString id_sc=index.data(Qt::UserRole).toString();
	QStringList list=id_sc.split("_");
	
	QString str = QString("DELETE FROM persones WHERE id_persones = %1").arg(list.value(1));
	
	if(!query.exec(str)){
	QMessageBox::StandardButton ret; ret = QMessageBox::critical (this,"Ошибка",("Удаление данных по персоналу не выполнено"),QMessageBox::Ok );
	 return;
	}
	UI->columnView->model()->removeRow(index.row(),index.parent()); 
	QMessageBox::StandardButton ret; ret = QMessageBox::information(this,"Предупреждение",("Удаление данных по персоналу выполнено"),QMessageBox::Ok );
	UI->property_object->setModel(0);
	UI->coord_object->setModel(0);
}

//============================================================================================
void Objectmanager::otchet()
{
    otch = new QDialog;

    QPushButton *Save_as_pdf = new QPushButton("Cохранить в PDF");

    connect(Save_as_pdf,SIGNAL(clicked()),this,SLOT(save_pdf()));

    otch->setWindowTitle("Формирование отчета");
    QGridLayout *Grid = new QGridLayout;

    name_region_edit = new QLineEdit();
    type_region_edit = new QLineEdit();
    description_region_edit = new QLineEdit();
    counte_population_edit = new QLineEdit();
    density_population_edit = new QLineEdit();
    emmigration_population_edit = new QLineEdit();
    immigration_population_edit = new QLineEdit();
    birth_population_edit = new QLineEdit();
    dead_population_edit = new QLineEdit();
    name_nations_edit = new QLineEdit();

    name_region_label = new QLabel("Название региона:");
    type_region_label = new QLabel("Тип региона:");
    description_region_label = new QLabel("Описание региона:");
    counte_population_label = new QLabel("Население региона:");
    density_population_label = new QLabel("Плотность населения региона:");
    emmigration_population_label = new QLabel("Уровень эммиграции:");
    immigration_population_label = new QLabel("Уровень иммиграции:");
    birth_population_label = new QLabel("Уровень рождаемости:");
    dead_population_label = new QLabel("Уровень смертности:");
    name_nations_label = new QLabel("Национальный состав:");

    QSqlQuery query;
    QString str = QString("SELECT name_region, type_region, description_region, counte_population, density_population, emmigration_population, immigration_population, birth_population, dead_population FROM region WHERE id_region = %1").arg(groud_id);
    query.exec(str);

    QSqlRecord data = query.record();

    while(query.next())
    {
        name_region_string = query.value(data.indexOf("name_region")).toString();
        type_region_string = query.value(data.indexOf("type_region")).toString();
        description_region_string = query.value(data.indexOf("description_region")).toString();
        counte_population_string = query.value(data.indexOf("counte_population")).toString();
        density_population_string = query.value(data.indexOf("density_population")).toString();
        emmigration_population_string = query.value(data.indexOf("emmigration_population")).toString();
        immigration_population_string = query.value(data.indexOf("immigration_population")).toString();
        birth_population_string = query.value(data.indexOf("birth_population")).toString();
        dead_population_string = query.value(data.indexOf("dead_population")).toString();
    }

    name_region_edit->setText(name_region_string);
    type_region_edit->setText(type_region_string);
    description_region_edit->setText(description_region_string);
    counte_population_edit->setText(QString("%1").arg(counte_population_string));
    density_population_edit->setText(density_population_string);
    emmigration_population_edit->setText(emmigration_population_string);
    immigration_population_edit->setText(immigration_population_string);
    birth_population_edit->setText(birth_population_string);
    dead_population_edit->setText(dead_population_string);

    Grid->addWidget(name_region_label,0,0);
    Grid->addWidget(name_region_edit,0,1);
    Grid->addWidget(type_region_label,1,0);
    Grid->addWidget(type_region_edit,1,1);
    Grid->addWidget(description_region_label,2,0);
    Grid->addWidget(description_region_edit,2,1);
    Grid->addWidget(counte_population_label,3,0);
    Grid->addWidget(counte_population_edit,3,1);
    Grid->addWidget(density_population_label,4,0);
    Grid->addWidget(density_population_edit,4,1);
    Grid->addWidget(emmigration_population_label,5,0);
    Grid->addWidget(emmigration_population_edit,5,1);
    Grid->addWidget(immigration_population_label,6,0);
    Grid->addWidget(immigration_population_edit,6,1);
    Grid->addWidget(birth_population_label,7,0);
    Grid->addWidget(birth_population_edit,7,1);
    Grid->addWidget(dead_population_label,8,0);
    Grid->addWidget(dead_population_edit,8,1);

    Grid->addWidget(Save_as_pdf,9,1);

    otch->setLayout(Grid);
    otch->show();
}

void Objectmanager::otchet_groups()
{
    QModelIndex index = UI->columnView->currentIndex();
    if(!index.data(Qt::UserRole).toBool()) return;
    QVariant id=index.data(Qt::UserRole);
    if (id.type() == QVariant::String){
        QString user_data=id.toString();
        QStringList list=user_data.split("_");
        if(list.value(0)=="dgr"){
         Reports *r = new Reports;
         int id_suka = list.value(1).toInt();
         QString report = r->create_object_formular(id_suka);
         r->show_preview_dialog(report);
        }
        else if(list.value(0)=="dsmi"){
            Reports *r = new Reports;
            int id_suka_smi = list.value(2).toInt();
            QString report = r->create_object_formular_smi(id_suka_smi);
            r->show_preview_dialog(report);
           }
        else if((list.value(0)=="chls") || (list.value(0) == "lss")){
            Reports *r = new Reports;
            int id_suka_ls = list.value(1).toInt();
            QString report = r->create_object_formular_ls(id_suka_ls);
            r->show_preview_dialog(report);
           }

}
}
// QString str = QString("select gr.name_groups,gr.counte_groups,gr.founder_group,gr.menegement_groups,gr.officce_groups,gr.description_groups,gr.propaganda_groups,tr.name_trend_groups,sph.name_sphere_groups, form.name_form_groups, reg.name_region FROM groups gr,trend_groups tr,sphere_groups sph, form_groups form, region reg where gr.id_trend=tr.id_trend_groups AND gr.id_sphere_groups=sph.id_sphere_groups AND gr.id_form_groups=form.id_form_groups AND gr.id_region = reg.id_region AND gr.id_groups=%1").arg(group_id);

void Objectmanager::save_pdf()
{
QTextDocument *doc = new QTextDocument;
text = new TextPrinter(this);
QDate date;
QString time_date;
int day,year,month;
date = date.currentDate();
day = date.dayOfWeek();
month = date.month();
year = date.year();
time_date = date.toString("dd." "MM" "yyyy.г");


QString htm;

htm.append("<HTML> <HEAD> </HEAD> <BODY> <H2> <CENTER> <B> Справка региона (района) </B> </CENTER> </H2>  ");
htm.append("  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Название региона: "); htm.append(name_region_string.toLocal8Bit());
htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Тип региона: "); htm.append(type_region_string.toLocal8Bit());
htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Описание региона: "); htm.append(description_region_string.toLocal8Bit());

htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > <CENTER> <B> 1.Население </B> </CENTER> ");

htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Население - "); htm.append(counte_population_string.toLocal8Bit());
htm.append(" чел.( "); htm.append(density_population_string.toLocal8Bit()); htm.append(" чел на км2 плотность населения )");
htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' >Национальный состав: ");

QString str,str1,str2;
QSqlQuery query,query1,query2;
QSqlRecord data,data1,data2;


str=QString("SELECT name_nations,persent_nations FROM region,ls_nations,nations WHERE region.id_region = ls_nations.id_region AND ls_nations.id_nations = nations.id_nations AND region.id_region = %1").arg(groud_id);
query.clear();
query.exec(str);
data.clear();
data = query.record();

number = counte_population_string.toInt();

while(query.next())
{
    name_nations_string = query.value(data.indexOf("name_nations")).toString();
    persent_nations = query.value(data.indexOf("persent_nations")).toDouble();
    number_nations = persent_nations/100*number;
    htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > -"); htm.append(name_nations_string.toLocal8Bit()); htm.append(" (");
    htm.append(QString("%1").arg(persent_nations).toLocal8Bit()); htm.append(" %, "); htm.append(QString("%1").arg(number_nations).toLocal8Bit()); htm.append(" чел)");
}



htm.append(" </FONT>  </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' >Уровень эммиграции: "); htm.append(emmigration_population_string.toLocal8Bit());
htm.append(" </FONT>  </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' >Уровень иммиграции: "); htm.append(immigration_population_string.toLocal8Bit());
htm.append(" </FONT>  </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' >Уровень рождаемости: "); htm.append(birth_population_string.toLocal8Bit());
htm.append(" </FONT>  </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' >Уровень смертности: "); htm.append(dead_population_string.toLocal8Bit());

number_sex_m = 0;
number_sex_w = 0;

number_m = 0;
number_w = 0;
QStringList list;

str=QString("SELECT persent_sex_m FROM pop_sex WHERE id_region = %1").arg(groud_id);
query.clear();
query.exec(str);
data.clear();
data = query.record();

while(query.next())
{
    number = query.value(data.indexOf("persent_sex_m")).toDouble();
    number_sex_m = number;
}



str=QString("SELECT persent_sex_w FROM pop_sex WHERE id_region = %1").arg(groud_id);
query.clear();
query.exec(str);
data.clear();
data = query.record();

while(query.next())
{
    number = query.value(data.indexOf("persent_sex_w")).toDouble();
    number_sex_w = number;
}
if(number_sex_m==0)
{
    htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Половой состав: Данных нет");

}
else
{
number = counte_population_string.toInt();

number_m = number_sex_m/100*number;




number_w = number - number_m;

htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Половой состав: ");
htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Мужской пол: "); htm.append(QString("%1").arg(number_m).toLocal8Bit()); htm.append(" чел. ("); htm.append(QString("%1").arg(number_sex_m).toLocal8Bit()); htm.append(" %)");
htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Женский пол: "); htm.append(QString("%1").arg(number_w).toLocal8Bit()); htm.append(" чел. ("); htm.append(QString("%1").arg(number_sex_w).toLocal8Bit()); htm.append(" %)");
}





str=QString("SELECT name_age,persent_age FROM age,pop_age WHERE pop_age.id_age = age.id_age AND id_region = %1").arg(groud_id);
query.clear();
query.exec(str);
data.clear();
data = query.record();
if(query.size()==0)
{
    htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Возрастной состав: Данных нет");
}

else
{
    htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Возрастной состав: ");

while(query.next())
{
    name_age_string = query.value(data.indexOf("name_age")).toString();
    persent_age_string = query.value(data.indexOf("persent_age")).toString();
    htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' >"); htm.append(name_age_string.toLocal8Bit()); htm.append(" - "); htm.append(persent_age_string.toLocal8Bit()); htm.append("%");
}
}




str=QString("SELECT name_confessions,persent_confessions FROM ls_confessions,confessions WHERE id_region = %1 AND ls_confessions.id_confessions = confessions.id_confessions").arg(groud_id);
query.clear();
query.exec(str);
data.clear();
data = query.record();
if(query.size()==0)
{
    htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Религиозный состав: Данных нет");
}

else
{
    htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Религиозный состав: ");

while(query.next())
{
    name_confessions = query.value(data.indexOf("name_confessions")).toString();
    persent_confessions = query.value(data.indexOf("persent_confessions")).toString();


    htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' >"); htm.append(name_confessions.toLocal8Bit()); htm.append(" - "); htm.append(persent_confessions.toLocal8Bit()); htm.append("%");
}
}


//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > <CENTER> <B> 2.СМИ </B> </CENTER> ");



int l=0;
int d=0;




str=QString("SELECT id_position_smi,name_position_smi FROM position_smi");
query.clear();
query.exec(str);
data.clear();
data = query.record();
QString name_position_smi,nametype_smi,name_smi;
int id_position_smi,id_type_smi;

while(query.next())
{
    name_position_smi = query.value(data.indexOf("name_position_smi")).toString();
    id_position_smi = query.value(data.indexOf("id_position_smi")).toInt();

    str1 = QString("SELECT id_type_smi,nametype_smi FROM type_smi");
    query1.clear();
    query1.exec(str1);
    data1.clear();
    data1 = query1.record();
    l=0;

    while(query1.next())
    {
     nametype_smi = query1.value(data1.indexOf("nametype_smi")).toString();
     id_type_smi = query1.value(data1.indexOf("id_type_smi")).toInt();

     str2=QString("SELECT smi.name_smi FROM smi,smi_region WHERE smi_region.id_smi = smi.id_smi AND id_position_smi = %1 AND id_type_smi = %2 AND  id_region = %3").arg(id_position_smi).arg(id_type_smi).arg(groud_id);
     query2.clear();
     query2.exec(str2);
     data2.clear();
     data2 = query2.record();
     smi_number = query2.size();
     if(smi_number>0)
     {
         l++;
         d=1;
         if(l<2)
         {
          htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > <CENTER> <B>");
          htm.append(name_position_smi.toLocal8Bit());
          htm.append(": </B> </CENTER> ");
         }
          htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > ");
          htm.append(nametype_smi.toLocal8Bit());
          htm.append(": ");
          htm.append(QString("%1").arg(smi_number).toLocal8Bit());
          while(query2.next())
          {
           name_smi = query2.value(data2.indexOf("name_smi")).toString();
           htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > - ");
           htm.append(name_smi.toLocal8Bit());
          }
     }
    }







    }



if(d==0) htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Данных нет  ");
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------

htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > <CENTER> <B> 3. Организации </B> </CENTER> ");

QString name_trend_groups,name_sphere_groups,name_groups;

int id_trend_groups,id_sphere_groups;
l=0;
d=0;

str=QString("SELECT id_trend_groups,name_trend_groups FROM trend_groups");
query.clear();
query.exec(str);
data.clear();
data = query.record();


while(query.next())
{
    name_trend_groups = query.value(data.indexOf("name_trend_groups")).toString();
    id_trend_groups = query.value(data.indexOf("id_trend_groups")).toInt();

    str1 = QString("SELECT id_sphere_groups,name_sphere_groups FROM sphere_groups");
    query1.clear();
    query1.exec(str1);
    data1.clear();
    data1 = query1.record();
    l=0;

    while(query1.next())
    {
     name_sphere_groups = query1.value(data1.indexOf("name_sphere_groups")).toString();
     id_sphere_groups = query1.value(data1.indexOf("id_sphere_groups")).toInt();

     str2=QString("SELECT name_groups FROM groups WHERE id_sphere_groups = %1 AND id_trend = %2 AND  id_region = %3").arg(id_sphere_groups).arg(id_trend_groups).arg(groud_id);
     query2.clear();
     query2.exec(str2);
     data2.clear();
     data2 = query2.record();
     smi_number = query2.size();
     if(smi_number>0)
     {
         l++;
         d=1;
         if(l<2)
         {
          htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > <CENTER> <B>");
          htm.append(name_trend_groups.toLocal8Bit());
          htm.append(": </B> </CENTER> ");
         }
          htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > ");
          htm.append(name_sphere_groups.toLocal8Bit());
          htm.append(": ");
          htm.append(QString("%1").arg(smi_number).toLocal8Bit());
          while(query2.next())
          {
           name_groups = query2.value(data2.indexOf("name_groups")).toString();
           htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > - ");
           htm.append(name_groups.toLocal8Bit());
          }
     }
    }







    }




if(d==0) htm.append(" </FONT>  </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Данных нет ");

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

str.clear();
query.clear();
data.clear();

str = QString("SELECT * FROM region WHERE id_region = %1").arg(groud_id);
query.exec(str);
data=query.record();

for(int i=0;i<33;i++) factori[i]=0;
factorflag=0;


while(query.next())
{
 int i=0;
 factori[i] = query.value(data.indexOf("poverty_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("price_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("education")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("trust_vs_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("support_vs_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("ability_vs_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("proposition_org_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("opposition_org_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("position_vip")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("unemployment_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("refugees")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("demography")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("availability_smi_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("smi_o_vs")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("protection_iti")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("ungov_org")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("patriotic_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("crim_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("corruption_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("shadow_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("illegal_migration_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("extremism_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("prison_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("protest_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("opg_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("drug_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("conflict_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("cooperation_ro")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("cult_object_population")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("autoritet_liders")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("regard_liders")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("religion_ls_enemy")).toDouble(); i++;
 factori[i] = query.value(data.indexOf("religion_ls_their")).toDouble();


}

factor_text << "Высокий уровень бедности и доли населения с денежными доходами, ниже региональной величины прожиточного минимума"
        << "Высокий уровень цен на продукты и услуги первой необходимости."
        << "Низкий уровень образования населения, недостаточное количество высших учебных заведений."
        << "Низкая степень доверия населения органам государственной власти, командованию Вооруженных Сил."
        << "Низкая степень поддержки населением действий Вооруженных Сил."
        << "Неспособность и отсутствие возможностей государственных (региональных) структур оказать содействие Вооружённым Силам в выполнении задач в период непосредственной угрозы агрессии и военное время."
        << "Низкая степень влияния на общественное мнение населения основных политических партий, неправительственных, общественных и религиозных организаций, выступающих в поддержку государства и Вооруженных сил."
        << "Наличие оппозиционных, радикальных политических движений и организаций (в том числе и молодежных) и достаточно высокий уровень их поддержки населением. "
        << "Негативная позиция государственных (региональных) авторитетных деятелей политики, культуры, искусства по отношению к Вооруженным Силам."
        << "Высокий уровень безработицы в регионе."
        << "Наличие беженцев из других регионов Российской Федерации, сопредельных с ней территорий."
        << "Сложная демографическая ситуация в регионе."
        << "Низкий уровень информатизации региона и степень доступности средств массовой информации и коммуникации, затрудняющие ведение пропаганды и контрпропаганды."
        << "Деструктивная направленность информации, публикуемой в региональных СМИ в отношении Вооруженных Сил."
        << "Низкая степень защищенности объектов телерадиовещания, сотовой связи, инфо - и телекоммуникационной инфраструктуры региона."
        << "Высокая степень активности неправительственных организаций и фондов деструктивной направленности."
        << "Низкий уровень сформированности патриотического сознания населения."
        << "Высокий уровень преступности в регионе."
        << "Высокая степень коррумпированности органов власти."
        << "Высокая степень влияния теневого сектора экономики и финансов региона на общественное мнение."
        << "Наличие нелегальных миграционных потоков."
        << "Наличие экстремистских проявлений и НВФ."
        << "Большое количество исправительно-трудовых учреждений, и число осужденных."
        << "Высокий уровень протестной активности населения."
        << "Наличие организованных преступных группировок."
        << "Наличие в peгионе путей незаконного экспорта (импорта) оружия и наркотиков."
        << "Наличие межнациональных, межэтнических конфликтов.";







for(int i=0;i<33;i++) if(factori[i]>0 && factori[i]<=0.3) factorflag=1;
if(factorflag == 0)
{
htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' >  <B> Факторы, дестабилизирующие моральную обстановку в регионе, отсутствуют, либо о них неизвестно. </B>  ");
}
else
{
    htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > <CENTER> <B> Факторами, дестабилизирующими моральную обстановку в регионе, являются: </B> </CENTER> ");


    for(int i=0;i<26;i++)
    {
        if(factori[i]>0 && factori[i]<=0.3)
        {
            htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > - ");
            htm.append(factor_text.at(i).toLocal8Bit());
        }
    }
}








htm.append(" </FONT> </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > <CENTER> <B> ВЫВОД </B> </CENTER> ");

if(rez_z_1 < 0.3)
htm.append(" </FONT>  </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Обстановка затрудняет выполнение задач ");
else
{
if(rez_z_1 > 0.3 && rez_z_1 <0.5)
htm.append(" </FONT>  </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Обстановка не оказывает существенного влияния на выполнение задач ");

else
htm.append(" </FONT>  </P>  <P> <BR> <FONT SIZE = '4' FACE = 'Arial' > Обстановка способствует выполнению задач ");

}


htm.append("  </FONT>  </P> </BODY> </HTML>");


QFile file_pdf;
QString filename;
QFont font;
font.setPointSizeF(10.10);
doc->setDefaultFont(font);
doc->setHtml(htm);


filename = QString("c:/formular.pdf");
file_pdf.setFileName(filename);
file_pdf.open(QIODevice::WriteOnly);
file_pdf.close();
text->setOrientation(QPrinter::Portrait);
text->exportPdf(doc,"Сохранить формуляр",filename);


otch->close();

}
int Objectmanager::calc_info_for_region(QString id_region)
{
    Calculate_K_omkrf calc;
    calc.set_parametr(id_region);
    calc.calc_Rezultat();
    float rez = calc.getRezultate();
    if(rez > 0 && rez < 0.3)
        return 1;
    if(rez >= 0.3 && rez < 0.5)
        return 2;
    if(rez >=0.5 && rez <= 0.8)
        return 3;
    if(rez == 0)
        return 0;
    if(rez < 0 || rez > 0.8)
        return -1;
    //return -678;
}

int Objectmanager::calc_mps_for_ls(float n)
{
    if(n > 0 && n < 0.3)
        return 1;
    if(n >= 0.3 && n < 0.6)
        return 2;
    if(n >=0.6 && n <= 1)
        return 3;
    if(n == 0)
        return 0;
    if(n < 0 || n > 1)
        return -1;
}
