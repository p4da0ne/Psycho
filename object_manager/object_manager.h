#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <QWidget>
#include <QTreeView>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QStandardItem>
#include <QPushButton>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QItemDelegate>
#include <QList>
#include <QBrush>
#include <QtSql>
#include <QDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QUrl>
#include <QPainter>
#include <QLineEdit>
#include "simledelegate.h"
#include <QProgressDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QAction>
#include "config_object_manager.h"
#include "add_elements_dialog.h"
#include "add_object.h"
#include "simledelegate.h"

class simledelegate;

namespace Ui
{
    class object_manager_form;
};

class __EXPORT_OBJECT_MANAGER Objectmanager: public QWidget

{
    Q_OBJECT

public:
	
    Objectmanager(QWidget *parent = 0);   //int id_object=0
    ~Objectmanager();

	void init_object_tree();
	QPoint		pe;
	
//void pravka(int);
public slots:
	void customMenuTree(const QPoint &);
	void customMenuView(const QPoint &);
	void add_country_blok();
	void delete_country_blok();
	
//signals:
//	void rightMousebuttonClicked();
private slots:
	void show_objects(const QModelIndex &index);
	void column_item_clicked ( const QModelIndex &index );
	void delete_region();
	void delete_smi();
	void delete_ls();
	void delete_groups();
	void delete_mpo();
	void delete_sc();
	void delete_pers();
	
private:
	simleDelegate *delegat; 
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QFont font);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path,QFont font);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QBrush b);
	QStandardItemModel * model;
	void region_click(int id_region);
	void smi_click(int id_smi);
	void ls_click(int id_ls);
	void gr_click(int id_gr);
	void mpo_click (int id_mpo);
	void sc_click(int id_sc);
	void add_region_components(QStandardItem *parent_item,int id_parent_object,int start_row);
	void add_ls_components(QStandardItem *parent_item,int id_parent_object,int start_row);
	void add_smi_components(QStandardItem *parent_item,int id_parent_object,int start_row);
	void add_groups_components(QStandardItem *parent_item,int id_parent_object,int start_row);
	void child_region_objects(QStandardItem *parent_item,int id_parent_region);
	void child_ls_objects(QStandardItem *parent_item,int id_parent_ls);
	void child_groups_objects(QStandardItem *parent_item,int id_gr);
	void child_smi_objects(QStandardItem *parent_item,int id_smi);
	void fill_combobox_country(QComboBox *box);
	void fill_combobox_blok(QComboBox *box, int id_current_blok);
			
	Ui::object_manager_form *UI;

//	protected:
//	void mousePressEvent(QMouseEvent *event);
	//virtual void contextMenuEvent(QContextMenuEvent* pe);
};

#endif 
