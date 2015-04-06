#ifndef CALCULATINGPROBLEMMANAGER_H
#define CALCULATINGPROBLEMMANAGER_H

#include <QWidget>
#include <QTreeView>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QStandardItem>
#include <QPushButton>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QList>

#include "Calculate_K_omkrf.h"
#include <QtSql>
#include <QMenu>
#include <QDir>
#include <QAction>
#include <QDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QUrl>
#include <QPoint>
#include <QPainter>
#include <QLineEdit>
#include <QStandardItem>
#include <QProgressDialog>
#include <QStringList>
#include "config_calculatingproblemmanager.h"
#include "calculating_mps.h"
#include "People_Losses.h"
#include "textprinter.h"
#include <QLabel>
#include <QTextDocument>
#include <QStringList>

namespace Ui
{
    class calculatingproblemmanager_form;
};

class __EXPORT_CALCULATINGPROBLEMMANAGER CalculatingProblemManager: public QWidget

{
    Q_OBJECT

public:
	
    CalculatingProblemManager(QWidget *parent = 0);   //int id_object=0
    ~CalculatingProblemManager();

	void init_object_tree();
//void pravka(int);
public slots:
	void customMenuTree(const QPoint &);
	void customMenuView(const QPoint &);

private slots:
	void show_objects(const QModelIndex &index);
	void column_item_clicked ( const QModelIndex &index );
	void otchet();
	void save_pdf();
	
private:
	Calculate_K_omkrf calc;
	calculating_mps calc_mps;
	QStringList iconsList;

	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QFont font);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path,QFont font);
	QStandardItemModel * model;
	int calc_info_for_region(QString id_region);
	int calc_mps_for_ls(float n);
	void region_click(int id_region);
	void smi_click(int id_smi);
	void ls_click(int id_ls);
	void gr_click(int id_gr);
	void add_region_components(QStandardItem *parent_item,int id_parent_object,int start_row);
	void add_ls_components(QStandardItem *parent_item,int id_parent_object,int start_row);
	void national_show_form(int id_region);
	void child_region_objects(QStandardItem *parent_item,int id_parent_region);
	void child_ls_objects(QStandardItem *parent_item,int id_parent_ls);

	QString name_region_string;
	QString type_region_string;
	QString description_region_string;
	QString counte_population_string;
	QString density_population_string;
	QString emmigration_population_string;
	QString immigration_population_string;
	QString birth_population_string;
	QString dead_population_string;
	QString name_nations_string;
	QString name_age_string;
	QString persent_age_string;
	QString name_confessions;
	QString persent_confessions;

	double factori[33];
	QStringList factor_text;
	
	
	int factorflag;

	QLineEdit *name_region_edit;
	QLineEdit *type_region_edit;
	QLineEdit *description_region_edit;
	QLineEdit *counte_population_edit;
	QLineEdit *density_population_edit;
	QLineEdit *emmigration_population_edit;
	QLineEdit *immigration_population_edit;
	QLineEdit *birth_population_edit;
	QLineEdit *dead_population_edit;
	QLineEdit *name_nations_edit;

	QLabel *name_region_label;
	QLabel *type_region_label;
	QLabel *description_region_label;
	QLabel *counte_population_label;
	QLabel *density_population_label;
	QLabel *emmigration_population_label;
	QLabel *immigration_population_label;
	QLabel *birth_population_label;
	QLabel *dead_population_label;
	QLabel *name_nations_label;

	float number_sex_m,number_sex_w,number,persent_nations,rez_z_1;
    int number_m,number_w,smi_number,number_nations;
	int groud_id;

	QDialog *otch;
	TextPrinter *text;
		
	Ui::calculatingproblemmanager_form *UI;
};

#endif 
