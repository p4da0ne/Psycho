#ifndef ADD_ELEMENTS_DIALOG_H
#define ADD_ELEMENTS_DIALOG_H

#include <QDialog>
#include <QtSql>
 class QCheckBox;
 class QDialogButtonBox;
 class QGroupBox;
 class QLabel;
 class QLineEdit;
 class QTextEdit;
 class QPushButton;
 class QComboBox;
 class QDateEdit;
#include <QTableWidget>
#include <QSqlRelationalTableModel>

class Add_elements_dialog : public QDialog
{
	Q_OBJECT

public:
	Add_elements_dialog(int type_element,int id_object,QWidget *parent=0);
	~Add_elements_dialog();
	QPushButton *deleteButton;

private:
	 void fill_combobox(QComboBox *in_comboBox,QString table,QStringList name_column);
	 void fill_combobox_nations(QComboBox *comboBox,int current_index);
	 void fill_combobox_conf(QComboBox *comboBox,int current_index);
	 void fill_combobox_prof(QComboBox *comboBox,int current_index);
	 void fill_combobox_age(QComboBox *comboBox,int current_index);
	 void fill_combobox_rank(QComboBox *comboBox,int current_index);
//	 void fill_combobox_pers(QComboBox *in_comboBox, QString table, QStringList name_column);
//	 void fill_combobox(QComboBox *in_comboBox, QString table, QString name_column, QString conditions);
	 int insert_in_table(QString table,QMap<QString,QString> map,QString id);
	 
	 QTextCodec *MainCodec;
     QLabel *label;
	 QLabel *label_2;
	 QLabel *label_3;
	 QLabel *label_4;
	 QLabel *label_5;
	 QLabel *label_6;
	 QLabel *label_7;
	 QLabel *label_8;
	 QLabel *label_9;
	 QLabel *label_10;
	 QLabel *label_11;
	 QLabel *label_12;
	 QLabel *label_13;
	 QLabel *label_14;
	 QLabel *label_15;
	 QLabel *label_16;
     QLabel *label_foto;
     QLabel *label_foto_name;

	 QTextEdit *textEdit;
	 QTextEdit *textEdit_2;
     QLineEdit *lineEdit;
	 QLineEdit *lineEdit_name;
	 QLineEdit *lineEdit_broad;
	 QLineEdit *lineEdit_site;
	 QTextEdit *textEdit_desc;
	 QLineEdit *lineEdit_freq;
	 QLineEdit *lineEdit_prot;
	 QLineEdit *lineEdit_counte;
	 QLineEdit *lineEdit_counte_ls;
	 QLineEdit *lineEdit_counte_ls_bd;
	 QLineEdit *lineEdit_counte_m;
	 QLineEdit *lineEdit_counte_w;
	 QLineEdit *lineEdit_counte_mpo_pso;
	 QLineEdit *lineEdit_aud;
	 QLineEdit *lineEdit_tirag;
	 QLineEdit *lineEdit_coef_1;
	 QLineEdit *lineEdit_coef_2;
	 QLineEdit *lineEdit_sem_1;
	 QLineEdit *lineEdit_sem_2;
	 QLineEdit *lineEdit_rank;
	 QLineEdit *lineEdit_aut;

	 QTextEdit *textEdit_founder;
	 QTextEdit *textEdit_menegm;
	 QTextEdit *textEdit_office;
	 QTextEdit *textEdit_descr;
	 QTextEdit *textEdit_propa;
	 QLineEdit *lineEdit_2;
	 QLineEdit *lineEdit_3;
	 QLineEdit *lineEdit_4;
	 QLineEdit *lineEdit_5;
	 QLineEdit *lineEdit_6;
	 QLineEdit *lineEdit_7;
	 QLineEdit *lineEdit_x;
	 QLineEdit *lineEdit_y;
     QCheckBox *caseCheckBox;
     QCheckBox *fromStartCheckBox;
     QCheckBox *wholeWordsCheckBox;
     QCheckBox *searchSelectionCheckBox;
     QCheckBox *backwardCheckBox;
     QDialogButtonBox *buttonBox;
     QPushButton *addButton;
	 QPushButton *addButtonn;
	 QPushButton *addButton_1;
	 QPushButton *saveButton;
	 
     QPushButton *cancelButton;

     QWidget *extension;
	 QComboBox *comboBox;
	 QComboBox *comboBox_2;
	 QComboBox *comboBox_3;
	 QComboBox *comboBox_4;
	 QComboBox *comboBox_5;
	 QComboBox *comboBox_6;
	 QComboBox *comboBox_7;
	 QComboBox *comboBox_8;
	 QComboBox *comboBox_9;
	 QComboBox *comboBox_10;
	 QCheckBox *checkbox;
	 QCheckBox *checkbox_enemy;
	 QComboBox *sexComboBox;
	 QComboBox *pavComboBox;
	 QComboBox *vul_pavComboBox;
	 QComboBox *itv_pavComboBox;
	 QDateEdit *date;

	 QTableWidget *directs_view;
     QTableWidget *confess_view;
	 QTableWidget *prof_view;
	 QTableWidget *age_view;
	 QTableWidget *sex_view;
	 QTableWidget *rank_view;
	 QTableWidget *coord_view;
	 QDialog *edit_dlg;
	 QDialog *add_dlg;
	
	 int in_id_object;
	 int id_persers;
	 

private slots:
	void add_smi();
	void add_name_smi();
	void add_mpo_pso();
	void add_mpo_pso_groups();
	void add_mpo_pso_smi();
	void add_groups();
	void add_persones_groups();
	void edit_persones();
	void add_persones_ls();
    void add_persones_smi();
	void add_special_cond();
	void add_coord_special_cond_dlg();
	void add_ls();
	void add_ls_parent();
	void show_redaktor(int,int);
	void show_redaktor_conf(int,int);
	void show_redaktor_prof(int,int);
	void show_redaktor_age(int,int);
	void show_redaktor_sex(int,int);
	void show_redaktor_rank(int,int);
	void show_redaktor_coord(int,int);
	void pravka_nations(int);
	void pravka_conf(int);
	void pravka_prof(int);
	void pravka_age(int);
	void pravka_sex(int);
	void pravka_rank(int);
	void table_nations();
	void table_nations_vf();
	void table_nations_gr();
	void add_nations_table_dlg();
	void add_nations_table_dlg_vf();
	void add_nations_table_dlg_gr();
	void add_confess_table_dlg();
	void add_confess_table_dlg_conf();
	void add_confess_table_dlg_gr();
	void add_prof_table_dlg();
	void add_prof_table_dlg_gr();
	void add_age_table_dlg();
	void add_age_table_dlg_gr();
	void add_sex_table_dlg();
	void add_sex_table_dlg_gr();
	void add_rank_table_dlg();
	void add_rank_table_dlg_gr();
	void clear_table(QTableWidget *table);
	void add_nations();
	void add_nations_vf();
	void add_nations_gr();
	void add_confess();
	void add_confess_conf();
	void add_confess_gr();
	void add_prof();
	void add_prof_gr();
	void add_age();
	void add_age_gr();
	void add_sex();
	void add_sex_gr();
	void add_rank();
	void add_rank_gr();
	void add_coord();
	void table_confess();
	void table_confess_conf();
	void table_confess_gr();
	void table_prof();
	void table_prof_gr();
	void table_age();
	void table_age_gr();
	void save_edit_persones();
	void table_sex();
	void table_sex_gr();
	void table_rank();
	void table_rank_gr();
	void table_coord(int id_coord);

	//void add_hardware_object();
	//void add_sstk_object();
	
};

#endif //  ADD_SOFTWARE_DIALOG_H
