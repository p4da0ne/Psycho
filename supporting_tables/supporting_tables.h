#ifndef SUPPORTING_TABLES_H
#define SUPPORTING_TABLES_H

#include "config_supporting_tables.h"
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QToolButton>
#include <QGroupBox>
#include <QTextCodec>
#include <QtSql>
#include <QTreeWidgetItem>
#include <QTableWidget>


namespace Ui
{
    class supporting_tables_form;
};


class __EXPORT_SUPPORTING_TABLES SupportingTables: public QDialog
{
    Q_OBJECT

public:
    SupportingTables(QDialog *parent = 0);
    virtual ~SupportingTables();
	
	void init_supporting_tree();
	QTreeWidgetItem * add_root(QString name, QString table_name);
	QTreeWidgetItem * add_child(QTreeWidgetItem *parent, QString name, QString table_name);
	void clear_tableWidget(QTableWidget *);
	QTreeWidgetItem *root_item;
	QTreeWidgetItem *ch_item;
	QTableWidget *patt_obj_table;


private slots:
    void fill_supp_table(QTreeWidgetItem * item, int column);
	void add_supp_table_row();
	void save_supp_table_data();
	void del_supp_table_data();
	void ok_slot();
	
private:
    Ui::supporting_tables_form *UI;

	QLabel *id;
	QLineEdit *obj_name_edit;
    QLineEdit *obj_short_name_edit;
	QLineEdit *classif_code_edit;
    QTextEdit *descr_obj;


	
};


#endif 
