#ifndef PERSONESDATA_H
#define PERSONESDATA_H

#include <QDialog>
#include <QComboBox>
#include <QStandardItemModel>
#include "comboboxdelegat.h"
#include <QSqlRelationalTableModel>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>

namespace Ui {
class PersonesData;
}

class PersonesData : public QDialog
{
    Q_OBJECT
    
public:
    explicit PersonesData(QString type_element, int id_object=0, int id_persones=0, QWidget *parent=0);
    ~PersonesData();
private slots:
    void    add_rank_history_row();
    void    del_rank_history_rows();
    void    add_educ_history_row();
    void    del_educ_history_rows();
    void    add_mil_educ_row();
    void    del_mil_educ_row();
    void    add_work_hist_row();
    void    del_work_hist_row();
    void    add_sc_rank_row();
    void    del_sc_rank_row();
    void    add_sc_rank_row_();
    void    del_sc_rank_row_();
    void    add_serv_row_();
    void    del_serv_row_();
    void    add_war_row_();
    void    del_war_row_();
    void    add_travma_row_();
    void    del_travma_row_();
    void    add_medal_row();
    void    del_medal_row();
    void    add_plen_row();
    void    del_plen_row();
    void    add_komp_row();
    void    del_komp_row();
    void    add_fam_row();
    void    del_fam_row();
    void    save_persones();
    void    open_file();
    void    add_label_fio(QString fio);
private:
    ComboboxDelegat *com_delegat;
    QStandardItemModel *fam_model;
    QStandardItemModel *rank_model;
    QStandardItemModel *educ_model;
    QStandardItemModel *mil_educ_model;
    QStandardItemModel *work_history_model;
    QStandardItemModel *science_rank_model;
    QStandardItemModel *science_rank_model_2;
    QStandardItemModel *serv_history_model;
    QStandardItemModel *war_act_model;
    QStandardItemModel *travma_model;
    QStandardItemModel *medal_model;
    QStandardItemModel *plen_model;
    QStandardItemModel *komp_model;
    void    insert_rank_history(int id_persones);
    void    insert_educ_history(int id_persones);
    void    insert_passport_data(int id_persones);
    void    insert_brak_data(int id_persones);
    void    insert_ulich_data(int id_persones);
    void    insert_work_history_data(int id_persones);
    void    insert_sc_rank_data(int id_persones);
    void    insert_service_data(int id_persones);
    void    insert_war_data(int id_persones);
    void    insert_travma_data(int id_persones);
    void    insert_medal_data(int id_persones);
    void    insert_plen_data(int id_persones);
    void    insert_komp_data(int id_persones);
    void    insert_fam_data(int id_persones);
	void	fillPersonesData(int id_persones);
	void	fillDataFromPersonesTable(int id_persones);
	void	updatePersonesTable();
	void	fillDocumentsData();

	void	fillModelFromDB(QString query,QStandardItemModel *model);
	void	fillFamModelFromDB(QString query,QStandardItemModel *model);
	

	int		id_persones;
    int     id_pers_group;
    int     id_pers_smi;
    int     id_pers_ls;
    int     id_object_pers;
    QString type_elem_pers;

    int     insert_in_table(QString table,QMap<QString,QString> map,QString id);
    void    fill_combobox_persones(QComboBox *box);

    Ui::PersonesData *UI;

};

#endif // PERSONESDATA_H
