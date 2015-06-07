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
#include <QAxWidget>
#include "config_object_manager.h"
#include "add_elements_dialog.h"
#include "personesdata.h"
#include "add_object.h"
#include "simledelegate.h"
#include "Calculate_K_omkrf.h"
#include "calculating_mps.h"
#include "People_Losses.h"
#include "textprinter.h"
#include <QProgressDialog>
#include <my_mapaccess.h>
#include "mylabel.h"
#include <QMouseEvent>
#include <QToolButton>
#include "persones_info.h"
#include "mysqlrelationaldelegate.h"

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
    void clear_tableWidget(QTableWidget *table);

    QMap<QString,QString> coord_line;
    QMap< QString, QMap<QString, QString> > object_map;
	QPoint		pe;
    HMAP hmap;
	MyLabel *label_foto; 
	QLabel *label_foto_hide;

public slots:
	void customMenuTree(const QPoint &);
	void customMenuView(const QPoint &);
	void add_country_blok();
	void delete_country_blok();

private slots:
    void update_one_click(const QModelIndex &index);
    void updateDB(QStandardItem* item);
    void show_nations_region();
    void show_nations_ls();
    void show_nations_gr();
    void show_confess_region();
    void show_confess_ls();
    void show_confess_gr();
    void show_profess_region();
    void show_profess_gr();
    void show_age_region();
    void show_age_gr();
    void show_sekas_region();
    void show_sekas_gr();
    void show_state_gr();
    void show_state_ls();
//================================================================================
    void add_new_blok();
    void get_path();
	void get_path_edit();
    void get_path_flag();
    void get_path_flag_edit();
    void delete_blok();
    void delete_country();
    void add_new_country();
    void edit_country_blok();
    void edit_country();
    void show_redaktor_blok(int row, int column);
    void show_redaktor_country(int row, int column);
	void show_objects(const QModelIndex &index);
	void column_item_clicked ( const QModelIndex &index );
	void delete_region();
	void delete_smi();
	void delete_ls();
	void delete_groups();
	void delete_mpo();
	void delete_sc();
	void delete_pers();
	void edit_persones();
	void clicked_open_file();
    void slotSearchObject();
    void otchet_groups();
    void add_new_coordinates();
    void show_dialog_add_file();
    void get_coordinates(int,QString,QString,QString);

    void change_coord_system(int);
    void delete_coordinates();
    void edit_coordinates_view();
    void WGS_to_other();
    void PLANE_to_other();

private:
    Persones_info *persInfo;
    QTableWidget *blok_edit_table;
    QTableWidget *country_edit_table;
    QDialog *edit_blok;
    QDialog *edit_country_dlg;
    QDialog *add_blok;
    QDialog *add_country;
    QDialog *edit_dlg;
    QLabel *path_lab;
    QLabel *blok_desc;
    QLabel *blok_name;
    QLineEdit *blok_filepath_edit;
    QTextEdit *blok_desc_edit;
    QLineEdit *blok_name_edit;
    QToolButton *path_button;
    QPushButton *ok_button;
    QPushButton *cancel_button;
	QDialog *edit_dlg_pers;
	QCheckBox *checkbox_enemy;
    void edit_coordinates(QString,int id_obj,QString,QString);
    int calcul(int);
    int count_smi(int);
    int id_persers;
	simleDelegate *delegat; 
    void fill_combobox_persones_(QComboBox *Box,int);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QFont font);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path,QFont font);
	QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QBrush b);
    QStandardItem * set_child_item(QString item_text,QString user_data,QStandardItem *parent_item,int row,QString icon_path,QBrush b);
    QStandardItemModel * model;
    QTableWidgetItem *item;
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
    void show_coordinates(QString,int,QString,QString);
    void table_blok();
    void table_country();
    Calculate_K_omkrf calc;
    calculating_mps calc_mps;
    int calc_info_for_region(QString id_region);
    int calc_mps_for_ls(float n);
    QStringList iconsList;

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

    QLineEdit *e1;
    QLineEdit *e2;
    QLineEdit *e3;
    QLineEdit *e4;
    QLineEdit *e5;
    QLineEdit *e6;
    QLineEdit *e15;
    QLineEdit *e16;


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
    int group_id;
    int smi_id;
    int ls_id;

    QDialog *otch;
    QDialog *otchet_gr;
    QDialog *add_coord;
    TextPrinter *text;

	Ui::object_manager_form *UI;

//	protected:
//	void mousePressEvent(QMouseEvent *event);
	//virtual void contextMenuEvent(QContextMenuEvent* pe);
};

#endif
