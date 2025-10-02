#include "personesdata.h"
#include <QSqlQuery>
#include <QFile>
#include <QFileDialog>
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include <QStandardItemModel>
#include "comboboxdelegat.h"
#include "ui_personesdata_form.h"


PersonesData::PersonesData(QString type_element,int id_object, int id_persones,QWidget *parent) :
    QDialog(parent),id_persones(id_persones),
    UI(new Ui::PersonesData)
{
    UI->setupUi(this);
	

//============= в/звания ==================================================================
    rank_model = new QStandardItemModel();
    UI->vzv_tableView->setModel(rank_model);
    QStringList header_rank_list;
    header_rank_list <<"Воинские звания \n (специальные звания, \n классные чины )"<<"Дата и номер"<<"Правовой акт о присвоении \n воинского звания \n (специального звания, классного чина)";
    rank_model->setHorizontalHeaderLabels(header_rank_list);
    UI->vzv_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//============= гражданское образование ==================================================================
    educ_model = new QStandardItemModel();
    UI->education_tableView->setModel(educ_model);
    QStringList educ_list;
    educ_list <<"Общеобразовательные учреждения, образовательные учреждения профессионального образования \n (указать в последовательном порядке наименования учебных заведений и год их окончания; \n наименования факультетов вузов; при неполном образовании - сколько классов или курсов окончил";
    educ_model->setHorizontalHeaderLabels(educ_list);
    UI->education_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//============= военное образование ==================================================================
    mil_educ_model = new QStandardItemModel();
    UI->vvuz_tableView->setModel(mil_educ_model);
    QStringList mil_educ_list;
    mil_educ_list <<"Военные образовательные учреждения профессионального образования \n (указать в последовательном порядке наименования военных образовательных учреждений профессионального образования \n и год их окончания; наименования факультетов и номера военно-учетных специальностей";
    mil_educ_model->setHorizontalHeaderLabels(mil_educ_list);
    UI->vvuz_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//============= самостоятельная трудовая деятельность ==================================================================
    work_history_model = new QStandardItemModel();
    UI->sampo_tableView->setModel(work_history_model);
    QStringList work_hist_list;
    work_hist_list <<"С какого времени (число,месяц,год)"<<"По какое время (число, месяц. год)"<<"Место работы, занимаемая должность"<<"Примечание";
    work_history_model->setHorizontalHeaderLabels(work_hist_list);
    UI->sampo_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//============= степень и звания ==================================================================
    science_rank_model = new QStandardItemModel();
    UI->science_tableView->setModel(science_rank_model);
    QStringList s_rank_list;
    s_rank_list <<"Ученая степень"<<"Дата присвоения";
    science_rank_model->setHorizontalHeaderLabels(s_rank_list);
    UI->science_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    science_rank_model_2 = new QStandardItemModel();
    UI->science_2_tableView->setModel(science_rank_model_2);
    QStringList s_rank_list_2;
    s_rank_list_2 <<"Ученое звание"<<"Дата присвоения";
    science_rank_model_2->setHorizontalHeaderLabels(s_rank_list_2);
    UI->science_2_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//=========================================================================================
    serv_history_model = new QStandardItemModel();
    UI->service_tableView->setModel(serv_history_model);
    QStringList serv_list;
    serv_list <<"С какого времени \n (число, месяц, год)"<<"По какое время \n (число, месяц, год)"<<"Должность, номер ВУС \n (код специальности) \n воинское звание (специальное звание)\n по штату, тарифный разряд \n (должностной оклад)"<<"Воинская часть, орган управления, \n учреждение, заведение, соединение, \nармия, группа войск, фронт или \n военный округ, флот или флотилия"<<"Чей приказ, дата \n и номер приказа";
    serv_history_model->setHorizontalHeaderLabels(serv_list);
    UI->service_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//=========================================================================================
    war_act_model = new QStandardItemModel();
    UI->bd_tableView->setModel(war_act_model);
    QStringList war_list;
    war_list <<"Участие в войнах и других боевых действиях"<<"С какого времени \n (число, месяц, год)"<<"По какое время \n (число, месяц, год)";
    war_act_model->setHorizontalHeaderLabels(war_list);
    UI->bd_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//=========================================================================================
    travma_model = new QStandardItemModel();
    UI->travm_tableView->setModel(travma_model);
    QStringList tr_list;
    tr_list <<"Увечья (ранения, травмы, контузии) и другие боевые поражения, их характер"<<"Когда и где получены";
    travma_model->setHorizontalHeaderLabels(tr_list);
    UI->travm_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//========================================================================================
    medal_model = new QStandardItemModel();
    UI->medal_tableView->setModel(medal_model);
    QStringList med_list;
    med_list <<"Наименование нагрудного знака"<<"За что награжден"<<"Чей приказ, его дата и номер";
    medal_model->setHorizontalHeaderLabels(med_list);
    UI->medal_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//========================================================================================
    plen_model = new QStandardItemModel();
    UI->plen_tableView->setModel(plen_model);
    QStringList pl_list;
    pl_list <<"Был ли в плену, при каких обстоятельствах, где и когда пленен и освобожден из плена";
    plen_model->setHorizontalHeaderLabels(pl_list);
    UI->plen_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//=========================================================================================
    komp_model = new QStandardItemModel();
    UI->kompromat_tableView->setModel(komp_model);
    QStringList k_list;
    k_list <<"Дата"<<"Обстоятельства";
    komp_model->setHorizontalHeaderLabels(k_list);
    UI->kompromat_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//=========================================================================================
    fam_model = new QStandardItemModel();
    UI->family_tableView->setModel(fam_model);
    QStringList f_list;
    f_list <<"Родственники"<<"Фамилия, имя, отчество, дата рождения жены, родителей, их место жительства. \n Имена и дата рождения детей, а также других иждивенцев";
   // fam_model->setH
    fam_model->setHorizontalHeaderLabels(f_list);
    UI->family_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    com_delegat = new ComboboxDelegat();

    UI->family_tableView->setItemDelegateForColumn(0,com_delegat);
//=========================================================================================

    id_object_pers = id_object;
    type_elem_pers = type_element;

    UI->name_persones->setVisible(false);

    connect(UI->last_name_lineEdit,SIGNAL(textChanged(QString)),this,SLOT(add_label_fio(QString)));
    connect(UI->name_lineEdit,SIGNAL(textChanged(QString)),this,SLOT(add_label_fio(QString)));
    connect(UI->mid_name_lineEdit,SIGNAL(textChanged(QString)),this,SLOT(add_label_fio(QString)));
    connect(UI->add_vzv_button,SIGNAL(clicked()),this,SLOT(add_rank_history_row()));
    connect(UI->del_vzv_button,SIGNAL(clicked()),this,SLOT(del_rank_history_rows()));
    connect(UI->add_education_button,SIGNAL(clicked()),this,SLOT(add_educ_history_row()));
    connect(UI->del_education_button,SIGNAL(clicked()),this,SLOT(del_educ_history_rows()));
    connect(UI->vvuz_add_pushButton,SIGNAL(clicked()),this,SLOT(add_mil_educ_row()));
    connect(UI->vvuz_del_pushButton,SIGNAL(clicked()),this,SLOT(del_mil_educ_row()));
    connect(UI->sampo_add_pushButton,SIGNAL(clicked()),this,SLOT(add_work_hist_row()));
    connect(UI->sampo_del_pushButton,SIGNAL(clicked()),this,SLOT(del_work_hist_row()));
    connect(UI->sc_add_pushButton,SIGNAL(clicked()),this,SLOT(add_sc_rank_row()));
    connect(UI->sc_del_pushButton,SIGNAL(clicked()),this,SLOT(del_sc_rank_row()));
    connect(UI->sc_2_add_pushButton,SIGNAL(clicked()),this,SLOT(add_sc_rank_row_()));
    connect(UI->sc_2_del_pushButton,SIGNAL(clicked()),this,SLOT(del_sc_rank_row_()));
    connect(UI->serv_add_pushButton,SIGNAL(clicked()),this,SLOT(add_serv_row_()));
    connect(UI->serv_del_pushButton,SIGNAL(clicked()),this,SLOT(del_serv_row_()));
    connect(UI->add_bd_pushButton,SIGNAL(clicked()),this,SLOT(add_war_row_()));
    connect(UI->del_bd_pushButton,SIGNAL(clicked()),this,SLOT(del_war_row_()));
    connect(UI->travm_add_pushButton,SIGNAL(clicked()),this,SLOT(add_travma_row_()));
    connect(UI->travm_del_pushButton,SIGNAL(clicked()),this,SLOT(del_travma_row_()));
    connect(UI->medal_add_pushButton,SIGNAL(clicked()),this,SLOT(add_medal_row()));
    connect(UI->medal_del_pushButton,SIGNAL(clicked()),this,SLOT(del_medal_row()));
    connect(UI->plen_add_pushButton,SIGNAL(clicked()),this,SLOT(add_plen_row()));
    connect(UI->plen_del_pushButton,SIGNAL(clicked()),this,SLOT(del_plen_row()));
    connect(UI->kompromat_add_pushButton,SIGNAL(clicked()),this,SLOT(add_komp_row()));
    connect(UI->kompromat_del_pushButton,SIGNAL(clicked()),this,SLOT(del_komp_row()));
    connect(UI->child_add_pushButton,SIGNAL(clicked()),this,SLOT(add_fam_row()));
    connect(UI->child_del_pushButton,SIGNAL(clicked()),this,SLOT(del_fam_row()));

 //===================== размеры полей и тд =========================================================

//    header_list<<" "<<" "<<"Воинские звания \n (специальные звания, \n классные чины )"<<"Правовой акт о присвоении \n воинского звания \n (специального звания, классного чина), \n его дата и номер"<<" ";

//    header_list_table_educ<<" "<<" "<<"Наименования учебных заведений"<<"Год окончания"<<"Наименования факультетов"<<" ";
//================================== CONNECT =======================================================

    connect(UI->save_all_button,SIGNAL(clicked()),this,SLOT(save_persones()));
    connect(UI->path_foto_toolButton,SIGNAL(clicked()),this,SLOT(open_file()));
//==================================================================================================
    fill_combobox_persones(UI->type_person_comboBox);
	fillMariageCombo();
//==================================================================================================

    QDate dateToday = QDate::currentDate();
    UI->hb_dateEdit->setDate(dateToday);
    UI->date_pass_dateEdit->setDate(dateToday);
    UI->date_udo_lich_dateEdit->setDate(dateToday);
    UI->brak_dateEdit->setDate(dateToday);

	if(id_persones != 0)
	{
		fillPersonesData(id_persones);
	}
}

PersonesData::~PersonesData()
{
    delete UI;
}


//=========================================================
void PersonesData::fillMariageCombo()
{
	UI->fam_comboBox->clear();
	QSqlQuery query;
	QString str = QString("SELECT id_mariage_status, status_name \
							FROM mariage_status ORDER BY id_mariage_status");

	if(query.exec(str))
	{
		QSqlRecord rec = query.record();
		while(query.next())
		{
			int id_status = query.value(rec.indexOf("id_mariage_status")).toInt();
			QString status_name = query.value(rec.indexOf("status_name")).toString();
			UI->fam_comboBox->addItem(status_name,id_status);
		}

	}

}


//======================================================
void PersonesData::fillPersonesData(int id_persones)
{
	fillDataFromPersonesTable(id_persones);
	fillDocumentsData();

	QString queryStr = QString("SELECT rank_name, rank_document, rank_get_date \
							   FROM rank_history WHERE id_persones = %1 ORDER BY id_rank_history").arg(id_persones); 
	fillModelFromDB(queryStr,rank_model);

	queryStr = QString("SELECT ei.info FROM education_institute ei, institute_type it \
						WHERE ei.id_institute_type = it.id_institute_type AND it.id_institute_type = 1 \
						AND ei.id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,educ_model);
	
	queryStr = QString("SELECT ei.info FROM education_institute ei, institute_type it \
						WHERE ei.id_institute_type = it.id_institute_type AND it.id_institute_type = 2 \
						AND ei.id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,mil_educ_model);

	queryStr = QString("SELECT begin_date, end_date, rank_and_place, comments \
						FROM work_history WHERE id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,work_history_model);

	queryStr = QString("SELECT sr.rank_name, sr.date FROM science_rank sr, science_rank_type srt \
						WHERE sr.id_science_rank_type = srt.id_science_rank_type \
						AND sr.id_science_rank_type = 1 AND sr.id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,science_rank_model);

	queryStr = QString("SELECT sr.rank_name, sr.date FROM science_rank sr, science_rank_type srt \
						WHERE sr.id_science_rank_type = srt.id_science_rank_type \
						AND sr.id_science_rank_type = 2 AND sr.id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,science_rank_model_2);

	queryStr = QString("SELECT begin_date, end_date, service_info, working_place, order_data \
						FROM service_history WHERE id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,serv_history_model);

	queryStr = QString("SELECT name, begin_date, end_date \
						FROM war_actions WHERE id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,war_act_model);

	queryStr = QString("SELECT name, comments FROM travm WHERE id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,travma_model);

	queryStr = QString("SELECT medal_name, achievment, \"order\" FROM medal WHERE id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,medal_model);

	queryStr = QString("SELECT comments FROM prison WHERE id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,plen_model);

	queryStr = QString("SELECT date, comments FROM compromat_table WHERE id_persones = %1").arg(id_persones); 
	fillModelFromDB(queryStr,komp_model);

	queryStr = QString("SELECT ft.id_family_types, ft.type_name, fd.info \
						FROM family_data fd, family_types ft \
						WHERE fd.id_family_types = ft.id_family_types \
						AND fd.id_persones = %1").arg(id_persones); 
	fillFamModelFromDB(queryStr,fam_model);

}


//---------------------------------------------------------
void PersonesData::fillDataFromPersonesTable(int id_persones)
{
	QSqlQuery query;
	QString str = QString("SELECT surname, name, patronumic, personal_number, nationality, birth_date, birth_place, rank_persones, id_type_persones, \
							finger_foto,languages, science_public, family_address, name_persones, id_mariage_status \
							FROM persones WHERE id_persones = %1").arg(id_persones);
	
	if(query.exec(str))
	{
		query.next();
		QSqlRecord rec = query.record(); 
		
		UI->last_name_lineEdit->setText(query.value(rec.indexOf("surname")).toString());
		UI->name_lineEdit->setText(query.value(rec.indexOf("name")).toString());
		UI->mid_name_lineEdit->setText(query.value(rec.indexOf("patronumic")).toString());
		UI->pers_numb_lineEdit->setText(query.value(rec.indexOf("personal_number")).toString());
		UI->nation_lineEdit->setText(query.value(rec.indexOf("nationality")).toString());
		UI->hb_dateEdit->setDate(query.value(rec.indexOf("birth_date")).toDate());
		UI->birth_plane_lineEdit->setText(query.value(rec.indexOf("birth_place")).toString());
		UI->rank_lineEdit->setText(query.value(rec.indexOf("rank_persones")).toString());
		UI->dakti_lineEdit->setText(query.value(rec.indexOf("finger_foto")).toString());
		UI->lang_textEdit->setPlainText(query.value(rec.indexOf("languages")).toString());
		UI->nauka_develop_textEdit->setPlainText(query.value(rec.indexOf("science_public")).toString());
		UI->adress_fam_lineEdit->setText(query.value(rec.indexOf("family_address")).toString());
		UI->name_persones->setText(query.value(rec.indexOf("name_persones")).toString());
		UI->fam_comboBox->setCurrentIndex(query.value(rec.indexOf("id_mariage_status")).toInt() - 1);
	}
}


void PersonesData::updatePersonesTable()
{
	QSqlQuery query;
	query.prepare("UPDATE persones SET surname=?, name=?, patronumic=?, personal_number=?, nationality=?, \
				  birth_date=?, birth_place=?, rank_persones=?, finger_foto=?, \
				  science_public=?, family_address=?, id_type_persones=?, languages=?, name_persones=?, id_mariage_status=? \
				  WHERE id_persones=?");
	query.addBindValue(UI->last_name_lineEdit->text());
	query.addBindValue(UI->name_lineEdit->text());
	query.addBindValue(UI->mid_name_lineEdit->text());
	query.addBindValue(UI->pers_numb_lineEdit->text());
	query.addBindValue(UI->nation_lineEdit->text());
	query.addBindValue(UI->hb_dateEdit->date().toString("yyyy-MM-dd"));
	query.addBindValue(UI->birth_plane_lineEdit->text());
	query.addBindValue(UI->rank_lineEdit->text());
	query.addBindValue(UI->dakti_lineEdit->text());
	query.addBindValue(UI->nauka_develop_textEdit->toPlainText());
	query.addBindValue(UI->adress_fam_lineEdit->text());
	int index = UI->type_person_comboBox->currentIndex();
	int id_type = UI->type_person_comboBox->itemData(index,Qt::UserRole).toInt();
	query.addBindValue(id_type);
	query.addBindValue(UI->lang_textEdit->toPlainText());
	query.addBindValue(UI->name_persones->text());

	int mar_index = UI->fam_comboBox->currentIndex();
	int id_mar_status = UI->fam_comboBox->itemData(mar_index,Qt::UserRole).toInt();
	query.addBindValue(id_mar_status);
	query.addBindValue(id_persones);
	
	if(!query.exec())
	{
		QString err = query.lastError().text();
	}
}

void PersonesData::fillDocumentsData()
{
	if(!id_persones) return;

	QSqlQuery query;
	QString str = QString("SELECT dt.id_document_type, dd.sequence, dd.number, dd.organ, dd.date \
							FROM document_data dd, document_type dt \
							WHERE dd.id_document_type = dt.id_document_type \
							AND dd.id_persones = %1 \
							ORDER BY dt.id_document_type").arg(id_persones);
	
	if(query.exec(str))
	{
		while(query.next())
		{
			QSqlRecord rec = query.record();
			if(query.value(rec.indexOf("id_document_type")).toInt() == 1)
			{
				UI->pass_ser_lineEdit->setText(query.value(rec.indexOf("sequence")).toString());
				UI->pass_numb_lineEdit->setText(query.value(rec.indexOf("number")).toString());
				UI->date_pass_dateEdit->setDate(query.value(rec.indexOf("date")).toDate());
				UI->vidacha_pass_lineEdit->setText(query.value(rec.indexOf("organ")).toString());
			}
			if(query.value(rec.indexOf("id_document_type")).toInt() == 2)
			{
				UI->udolich_ser_lineEdit->setText(query.value(rec.indexOf("sequence")).toString());
				UI->udolich_numb_lineEdit->setText(query.value(rec.indexOf("number")).toString());
				UI->date_udo_lich_dateEdit->setDate(query.value(rec.indexOf("date")).toDate());
				UI->vidacha_udo_lich_lineEdit->setText(query.value(rec.indexOf("organ")).toString());
			}
			if(query.value(rec.indexOf("id_document_type")).toInt() == 3)
			{
				UI->brak_seq_lineEdit->setText(query.value(rec.indexOf("sequence")).toString());
				UI->brak_numb_lineEdit->setText(query.value(rec.indexOf("number")).toString());
				UI->brak_dateEdit->setDate(query.value(rec.indexOf("date")).toDate());
				UI->brak_kem_lineEdit->setText(query.value(rec.indexOf("organ")).toString());
			}
		}
	}
}

//======================================================
void PersonesData::add_rank_history_row()
{
    QList<QStandardItem*> itemList;

    if(rank_model->rowCount() > 0)
    {
        QModelIndex index = rank_model->index(rank_model->rowCount()-1,0);
        if(rank_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    rank_model->appendRow(itemList);


}

void PersonesData::del_rank_history_rows()
{
    for(int row=rank_model->rowCount()-1;row>=0;row--)
    {
        if(rank_model->item(row)->checkState() == Qt::Checked)
        {
            rank_model->removeRow(row);
        }
    }
}

void PersonesData::add_educ_history_row()
{
    QList<QStandardItem*> itemList;

    if(educ_model->rowCount() > 0)
    {
        QModelIndex index = educ_model->index(educ_model->rowCount()-1,0);
        if(educ_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    educ_model->appendRow(itemList);

}

void PersonesData::del_educ_history_rows()
{
    for(int row=educ_model->rowCount()-1;row>=0;row--)
    {
        if(educ_model->item(row)->checkState() == Qt::Checked)
        {
            educ_model->removeRow(row);
        }
    }
}

void PersonesData::add_mil_educ_row()
{
    QList<QStandardItem*> itemList;

    if(mil_educ_model->rowCount() > 0)
    {
        QModelIndex index = mil_educ_model->index(mil_educ_model->rowCount()-1,0);
        if(mil_educ_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);
    itemList.append(item);

    mil_educ_model->appendRow(itemList);
}

void PersonesData::del_mil_educ_row()
{
    for(int row=mil_educ_model->rowCount()-1;row>=0;row--)
    {
        if(mil_educ_model->item(row)->checkState() == Qt::Checked)
        {
            mil_educ_model->removeRow(row);
        }
    }
}

void PersonesData::add_work_hist_row()
{
    QList<QStandardItem*> itemList;

    if(work_history_model->rowCount() > 0)
    {
        QModelIndex index = work_history_model->index(work_history_model->rowCount()-1,0);
        if(work_history_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    work_history_model->appendRow(itemList);
}

void PersonesData::del_work_hist_row()
{
    for(int row=work_history_model->rowCount()-1;row>=0;row--)
    {
        if(work_history_model->item(row)->checkState() == Qt::Checked)
        {
            work_history_model->removeRow(row);
        }
    }
}

void PersonesData::add_sc_rank_row()
{
    QList<QStandardItem*> itemList;

    if(science_rank_model->rowCount() > 0)
    {
        QModelIndex index = science_rank_model->index(science_rank_model->rowCount()-1,0);
        if(science_rank_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    science_rank_model->appendRow(itemList);
}

void PersonesData::del_sc_rank_row()
{
    for(int row=science_rank_model->rowCount()-1;row>=0;row--)
    {
        if(science_rank_model->item(row)->checkState() == Qt::Checked)
        {
            science_rank_model->removeRow(row);
        }
    }
}

void PersonesData::add_sc_rank_row_()
{
    QList<QStandardItem*> itemList;

    if(science_rank_model_2->rowCount() > 0)
    {
        QModelIndex index = science_rank_model_2->index(science_rank_model_2->rowCount()-1,0);
        if(science_rank_model_2->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    science_rank_model_2->appendRow(itemList);
}

void PersonesData::del_sc_rank_row_()
{
    for(int row=science_rank_model_2->rowCount()-1;row>=0;row--)
    {
        if(science_rank_model_2->item(row)->checkState() == Qt::Checked)
        {
            science_rank_model_2->removeRow(row);
        }
    }
}

void PersonesData::add_serv_row_()
{
    QList<QStandardItem*> itemList;

    if(serv_history_model->rowCount() > 0)
    {
        QModelIndex index = serv_history_model->index(serv_history_model->rowCount()-1,0);
        if(serv_history_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    serv_history_model->appendRow(itemList);
}

void PersonesData::del_serv_row_()
{
    for(int row=serv_history_model->rowCount()-1;row>=0;row--)
    {
        if(serv_history_model->item(row)->checkState() == Qt::Checked)
        {
            serv_history_model->removeRow(row);
        }
    }
}

void PersonesData::add_war_row_()
{
    QList<QStandardItem*> itemList;

    if(war_act_model->rowCount() > 0)
    {
        QModelIndex index = war_act_model->index(war_act_model->rowCount()-1,0);
        if(war_act_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);


    war_act_model->appendRow(itemList);
}

void PersonesData::del_war_row_()
{
    for(int row=war_act_model->rowCount()-1;row>=0;row--)
    {
        if(war_act_model->item(row)->checkState() == Qt::Checked)
        {
            war_act_model->removeRow(row);
        }
    }
}

void PersonesData::add_travma_row_()
{
    QList<QStandardItem*> itemList;

    if(travma_model->rowCount() > 0)
    {
        QModelIndex index = travma_model->index(travma_model->rowCount()-1,0);
        if(travma_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    travma_model->appendRow(itemList);
}

void PersonesData::del_travma_row_()
{
    for(int row=travma_model->rowCount()-1;row>=0;row--)
    {
        if(travma_model->item(row)->checkState() == Qt::Checked)
        {
            travma_model->removeRow(row);
        }
    }
}

void PersonesData::add_medal_row()
{
    QList<QStandardItem*> itemList;

    if(medal_model->rowCount() > 0)
    {
        QModelIndex index = medal_model->index(medal_model->rowCount()-1,0);
        if(medal_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    medal_model->appendRow(itemList);
}

void PersonesData::del_medal_row()
{
    for(int row=medal_model->rowCount()-1;row>=0;row--)
    {
        if(medal_model->item(row)->checkState() == Qt::Checked)
        {
            medal_model->removeRow(row);
        }
    }
}

void PersonesData::add_plen_row()
{
    QList<QStandardItem*> itemList;

    if(plen_model->rowCount() > 0)
    {
        QModelIndex index = plen_model->index(plen_model->rowCount()-1,0);
        if(plen_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    plen_model->appendRow(itemList);
}

void PersonesData::del_plen_row()
{
    for(int row=plen_model->rowCount()-1;row>=0;row--)
    {
        if(plen_model->item(row)->checkState() == Qt::Checked)
        {
            plen_model->removeRow(row);
        }
    }
}

void PersonesData::add_komp_row()
{
    QList<QStandardItem*> itemList;

    if(komp_model->rowCount() > 0)
    {
        QModelIndex index = komp_model->index(komp_model->rowCount()-1,0);
        if(komp_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData("",Qt::DisplayRole);

    itemList.append(item);

    item = new QStandardItem("");
    itemList.append(item);

    komp_model->appendRow(itemList);
}

void PersonesData::del_komp_row()
{
    for(int row=komp_model->rowCount()-1;row>=0;row--)
    {
        if(komp_model->item(row)->checkState() == Qt::Checked)
        {
            komp_model->removeRow(row);
        }
    }
}

void PersonesData::add_fam_row()
{
    QList<QStandardItem*> itemList;

    if(fam_model->rowCount() > 0)
    {
        QModelIndex index = fam_model->index(fam_model->rowCount()-1,0);
        if(fam_model->data(index,Qt::DisplayRole).toString().isEmpty())
        {
            return;
        }
    }

    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);


    fam_model->appendRow(item);


}

void PersonesData::del_fam_row()
{
    for(int row=fam_model->rowCount()-1;row>=0;row--)
    {
        if(fam_model->item(row)->checkState() == Qt::Checked)
        {
            fam_model->removeRow(row);
        }
    }
}

void PersonesData::save_persones()
{
	int id_rezult;
    if(id_persones)
	{
		updatePersonesTable();
	}
	else
	{
	
		int id_type_pers_=UI->type_person_comboBox->itemData(UI->type_person_comboBox->currentIndex()).toInt();
		
		int mar_index = UI->fam_comboBox->currentIndex();
		int id_mariage = UI->fam_comboBox->itemData(mar_index,Qt::UserRole).toInt();
	
		QString f_name = UI->last_name_lineEdit->text();
		QString n_name = UI->name_lineEdit->text();
		QString o_name = UI->mid_name_lineEdit->text();
		QString pers_name = UI->name_persones->text();
		QString pers_numb = UI->pers_numb_lineEdit->text();
		QString nations = UI->nation_lineEdit->text();
		QString b_date = UI->hb_dateEdit->date().toString("dd-MM-yyyy");
		QString birth_place = UI->birth_plane_lineEdit->text();
		QString rank_pers = UI->rank_lineEdit->text();
		QString fing = UI->dakti_lineEdit->text();
		QString lang = UI->lang_textEdit->toPlainText();
		QString sc = UI->nauka_develop_textEdit->toPlainText();
		QString fam_adress= UI->adress_fam_lineEdit->text();

		QMap<QString,QString> map;

        if (id_object_pers<1) return;
        map.clear();
        map.insert("surname",f_name);
        map.insert("name",n_name);
        map.insert("patronumic",o_name);
        map.insert("name_persones",pers_name);
        map.insert("personal_number",pers_numb);
        map.insert("nationality",nations);
        map.insert("birth_date",b_date);
        map.insert("birth_place",birth_place);
        map.insert("rank_persones",rank_pers);
        map.insert("finger_foto",fing);
        map.insert("id_mariage_status",QString::number(id_mariage));
        map.insert("languages",lang);
        map.insert("science_public",sc);
        map.insert("family_address",fam_adress);

        if (type_elem_pers == "group"){

            id_pers_group = id_object_pers;
            map.insert("id_groups",QString::number(id_pers_group));
        }
        else if (type_elem_pers == "smi"){

            id_pers_smi = id_object_pers;
            map.insert("id_smi",QString::number(id_pers_smi));
        }
        else {

            id_pers_ls = id_object_pers;
            map.insert("id_ls",QString::number(id_pers_ls));
        }
        map.insert("id_type_persones",QString::number(id_type_pers_));

        id_rezult=insert_in_table("persones",map,"id_persones");


		id_persones = id_rezult;
	}
	    insert_passport_data(id_persones);
        insert_ulich_data(id_persones);
        insert_rank_history(id_persones);
        insert_educ_history(id_persones);
        insert_work_history_data(id_persones);
        insert_sc_rank_data(id_persones);
        insert_service_data(id_persones);
        insert_war_data(id_persones);
        insert_travma_data(id_persones);
        insert_medal_data(id_persones);
        insert_plen_data(id_persones);
        insert_komp_data(id_persones);
        insert_brak_data(id_persones);
        insert_fam_data(id_persones);
//======================= фото персоны ===================================
	
		bool fotoFlag = isExistPhotoInDB(id_persones);

		QFile file(UI->path_foto_lineEdit->text());
        if(!file.open(QIODevice::ReadOnly))
		{
			if(!fotoFlag)
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("Внимание");
				msgBox.setText("Фотография отсутствует.Нажмите ДА, чтобы продолжить");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
				msgBox.setButtonText(QMessageBox::Yes, "Да");
				msgBox.setButtonText(QMessageBox::No, "Нет");
				if (msgBox.exec()== QMessageBox::Yes){

					if(id_rezult>0){
						this->done(id_rezult);
					}else{
						this->done(0);
					}
				  }
				else{
					return;
				}		
			}
		}
		else
		{
			QSqlQuery query;

			query.prepare("UPDATE persones SET image_persones = ? WHERE id_persones = ?");

			QByteArray image_persones = file.readAll();
			query.addBindValue(image_persones);
			query.addBindValue(id_persones);
			if(!query.exec())
			{
				QString s = query.lastError().text();
			}
		}

        if(id_rezult>0){
            this->done(id_rezult);
        }else{
            this->done(0);
        }
	
}

//=======================================================================================
bool PersonesData::isExistPhotoInDB(int idPersones)
{
	if(!id_persones) return false;
	QString query_str;
	
	query_str = QString("SELECT image_persones FROM persones WHERE id_persones = %1").arg(idPersones);

	QSqlQuery query;
	if(query.exec(query_str))
	{
		QSqlRecord rec = query.record();
		query.next();
		if(query.value(rec.indexOf("image_persones")).toBool() == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}



//=======================================================================================
void PersonesData::fillModelFromDB(QString query_str,QStandardItemModel *model)
{
	QSqlQuery query;
	if(query.exec(query_str))
	{
		QSqlRecord rec = query.record();
		int colCount = rec.count();
		while(query.next())
		{
			QList<QStandardItem *> rowList;
			for(int col=0;col<colCount;col++)
			{
				QStandardItem *item = new QStandardItem(query.value(col).toString());
				if(col == 0)
				{
					item->setCheckable(true);
					item->setCheckState(Qt::Unchecked);
				}
				rowList.append(item);
			}
			model->appendRow(rowList);
		}
	}
}


//=======================================================================================
void PersonesData::fillFamModelFromDB(QString query_str,QStandardItemModel *model)
{
	QSqlQuery query;
	if(query.exec(query_str))
	{
		QSqlRecord rec = query.record();
		int colCount = rec.count();
		while(query.next())
		{
			QList<QStandardItem *> rowList;
			QStandardItem *item = new QStandardItem;
			item->setCheckable(true);
			item->setCheckState(Qt::Unchecked);
			item->setData(query.value(0).toInt(),Qt::UserRole);
			item->setData(query.value(1).toString(),Qt::DisplayRole);
			rowList.append(item);

			item = new QStandardItem;
			item->setData(query.value(2).toString(),Qt::DisplayRole);
			rowList.append(item);
					
			model->appendRow(rowList);
		}
	}
}



//======================= выбор типа персоналии в комбо ========================================
void PersonesData::fill_combobox_persones(QComboBox *box)
{
    box->clear();
    QSqlQuery query;
   // box->addItem("-");
    query.exec("SELECT id_type_persones, name_type_persones FROM type_persones");
    while (query.next())
    {
        QString name_type = query.value(1).toString();
        int id_type = query.value(0).toInt();
        box->addItem(name_type,id_type);
    }
    query.clear();
}
//====== данные по персоналиям для всех ** организации ** сми ** ВФ ** =========================
int PersonesData::insert_in_table(QString table,QMap<QString,QString> map,QString id){

        QString table_columns="";
        QString columns_values="";
        QMapIterator<QString, QString> i(map);
        while (i.hasNext()) {
             i.next();
             table_columns += i.key() + ",";
             columns_values += "'" + i.value() + "',";
        }
        table_columns.chop(1);
        columns_values.chop(1);
        QSqlQuery query;
        QString str = QString("insert into %1 (%2) values (%3) returning %4").arg(table).arg(table_columns).arg(columns_values).arg(id);
        bool res=query.exec(str);
        if(res){
            while(query.next()){
            int last_id= query.value(0).toInt();
            query.clear();
            return last_id;
            }
        }else{
            query.clear();
            return 0;
        }
       // insert into persones (birth_date,id_smi,id_type_persones,name,name_persones,nationality,patronumic,personal_number,surname) values ('01-06-2015','62','0','sdf','sdfdsf s.s.','sdf','sdf','sdf','sdfdsf') returning id_persones
}
//=========================== путь к файлу с фото ==================================
void PersonesData::open_file(){
    QString fileName = QFileDialog::getOpenFileName(this, "Выбор фотографии", "",
                                                    "Images (*.jpg *.png)");
    if (fileName.isEmpty()) return;
    UI->path_foto_lineEdit->setText(fileName);
    this->raise();
}

void PersonesData::add_label_fio(QString fio){

    QString f = UI->last_name_lineEdit->text();
    QString n = UI->name_lineEdit->text();
    QString m = UI->mid_name_lineEdit->text();
    int s = m.length();
    int d = n.length();
    m.chop(s-1);
    n.chop(d-1);
    if (m.isEmpty() && (n.isEmpty())){
        fio = f;
        UI->name_persones->setText(fio);
        return;
    }
    else if (m.isEmpty()){
        fio = f +" " + n +".";
        UI->name_persones->setText(fio);
        return;
    }
    else{
        fio = f +" " + n +"." + m +".";
        UI->name_persones->setText(fio);
    }
}

void PersonesData::insert_rank_history(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM rank_history WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }


    for(int row=0;row<rank_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO rank_history (id_persones,rank_name,rank_document,rank_get_date) VALUES (?,?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(rank_model->data(rank_model->index(row,0),Qt::DisplayRole).toString());
        query.addBindValue(rank_model->data(rank_model->index(row,1),Qt::DisplayRole).toString());
        query.addBindValue(rank_model->data(rank_model->index(row,2),Qt::DisplayRole).toString());
        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }

}

void PersonesData::insert_educ_history(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM education_institute WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }
    int i=1;

    for(int row=0;row<educ_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO education_institute (id_persones,id_institute_type,info) VALUES (?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(i);
        query.addBindValue(educ_model->data(educ_model->index(row,0),Qt::DisplayRole).toString());

        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }
    int y=2;

    for(int row=0;row<mil_educ_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO education_institute (id_persones,id_institute_type,info) VALUES (?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(y);
        query.addBindValue(mil_educ_model->data(mil_educ_model->index(row,0),Qt::DisplayRole).toString());

        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }


}

void PersonesData::insert_passport_data(int id_persones)
{

    QSqlQuery query;
    query.prepare("DELETE FROM document_data WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }
	
	QString b_date = UI->date_pass_dateEdit->date().toString("dd-MM-yyyy");
    int i = 1;
	query.clear();

    query.prepare("INSERT INTO document_data (sequence, number, date, organ, id_persones, id_document_type) VALUES (?,?,?,?,?,?)");
    query.addBindValue(UI->pass_ser_lineEdit->text());
    query.addBindValue(UI->pass_numb_lineEdit->text());
    query.addBindValue(b_date);
    query.addBindValue(UI->vidacha_pass_lineEdit->text());
    query.addBindValue(id_persones);
    query.addBindValue(i);

    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }

}

void PersonesData::insert_brak_data(int id_persones)
{
    
	QString b_date = UI->brak_dateEdit->date().toString("dd-MM-yyyy");
    int i = 3;

    QSqlQuery query;

    query.prepare("INSERT INTO document_data (sequence, number, date, organ, id_persones, id_document_type) VALUES (?,?,?,?,?,?)");
    query.addBindValue(UI->brak_seq_lineEdit->text());
    query.addBindValue(UI->brak_numb_lineEdit->text());
    query.addBindValue(b_date);
    query.addBindValue(UI->brak_kem_lineEdit->text());
    query.addBindValue(id_persones);
    query.addBindValue(i);

    if(!query.exec()){

        QString str = query.lastError().databaseText();
        return;
    }
}
void PersonesData::insert_ulich_data(int id_persones)
{

    QString b_date = UI->date_udo_lich_dateEdit->date().toString("dd-MM-yyyy");
    int i = 2;

    QSqlQuery query;

    query.prepare("INSERT INTO document_data (sequence, number, date, organ, id_persones, id_document_type) VALUES (?,?,?,?,?,?)");
    query.addBindValue(UI->udolich_ser_lineEdit->text());
    query.addBindValue(UI->udolich_numb_lineEdit->text());
    query.addBindValue(b_date);
    query.addBindValue(UI->vidacha_udo_lich_lineEdit->text());
    query.addBindValue(id_persones);
    query.addBindValue(i);

    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }

}

void PersonesData::insert_work_history_data(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM work_history WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }


    for(int row=0;row<work_history_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO work_history (id_persones,begin_date,end_date,rank_and_place,comments) VALUES (?,?,?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(work_history_model->data(work_history_model->index(row,0),Qt::DisplayRole).toString());
        query.addBindValue(work_history_model->data(work_history_model->index(row,1),Qt::DisplayRole).toString());
        query.addBindValue(work_history_model->data(work_history_model->index(row,2),Qt::DisplayRole).toString());
        query.addBindValue(work_history_model->data(work_history_model->index(row,3),Qt::DisplayRole).toString());
        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }

}

void PersonesData::insert_sc_rank_data(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM science_rank WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }
    int i=1;

    for(int row=0;row<science_rank_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO science_rank (id_persones,id_science_rank_type,rank_name,date) VALUES (?,?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(i);
        query.addBindValue(science_rank_model->data(science_rank_model->index(row,0),Qt::DisplayRole).toString());
        query.addBindValue(science_rank_model->data(science_rank_model->index(row,1),Qt::DisplayRole).toString());

        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }
    int y=2;

    for(int row=0;row<science_rank_model_2->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO science_rank (id_persones,id_science_rank_type,rank_name,date) VALUES (?,?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(y);
        query.addBindValue(science_rank_model_2->data(science_rank_model_2->index(row,0),Qt::DisplayRole).toString());
        query.addBindValue(science_rank_model_2->data(science_rank_model_2->index(row,1),Qt::DisplayRole).toString());

        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }

}

void PersonesData::insert_service_data(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM service_history WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }


    for(int row=0;row<serv_history_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO service_history (id_persones,begin_date,end_date,service_info,working_place,order_data) VALUES (?,?,?,?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(serv_history_model->data(serv_history_model->index(row,0),Qt::DisplayRole).toString());
        query.addBindValue(serv_history_model->data(serv_history_model->index(row,1),Qt::DisplayRole).toString());
        query.addBindValue(serv_history_model->data(serv_history_model->index(row,2),Qt::DisplayRole).toString());
        query.addBindValue(serv_history_model->data(serv_history_model->index(row,3),Qt::DisplayRole).toString());
        query.addBindValue(serv_history_model->data(serv_history_model->index(row,4),Qt::DisplayRole).toString());
        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }
}

void PersonesData::insert_war_data(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM war_actions WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }


    for(int row=0;row<war_act_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO war_actions (id_persones,name,begin_date,end_date) VALUES (?,?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(war_act_model->data(war_act_model->index(row,0),Qt::DisplayRole).toString());
        query.addBindValue(war_act_model->data(war_act_model->index(row,1),Qt::DisplayRole).toString());
        query.addBindValue(war_act_model->data(war_act_model->index(row,2),Qt::DisplayRole).toString());
        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }
}

void PersonesData::insert_travma_data(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM travm WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }


    for(int row=0;row<travma_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO travm (id_persones,name,comments) VALUES (?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(travma_model->data(travma_model->index(row,0),Qt::DisplayRole).toString());
        query.addBindValue(travma_model->data(travma_model->index(row,1),Qt::DisplayRole).toString());
        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }
}

void PersonesData::insert_medal_data(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM medal WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }


    for(int row=0;row<medal_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO medal (id_persones,medal_name,achievment,\"order\") VALUES (?,?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(medal_model->data(medal_model->index(row,0),Qt::DisplayRole).toString());
        query.addBindValue(medal_model->data(medal_model->index(row,1),Qt::DisplayRole).toString());
        query.addBindValue(medal_model->data(medal_model->index(row,2),Qt::DisplayRole).toString());
        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }
}

void PersonesData::insert_plen_data(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM prison WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }


    for(int row=0;row<plen_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO prison (id_persones,comments) VALUES (?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(plen_model->data(plen_model->index(row,0),Qt::DisplayRole).toString());

        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }
}

void PersonesData::insert_komp_data(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM compromat_table WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }


    for(int row=0;row<komp_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO compromat_table (id_persones,date,comments) VALUES (?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(komp_model->data(komp_model->index(row,0),Qt::DisplayRole).toString());
        query.addBindValue(komp_model->data(komp_model->index(row,1),Qt::DisplayRole).toString());

        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }
}

void PersonesData::insert_fam_data(int id_persones)
{
    QSqlQuery query;
    query.prepare("DELETE FROM family_data WHERE id_persones = ?");
    query.addBindValue(id_persones);
    if(!query.exec())
    {
        QString str = query.lastError().databaseText();
        return;
    }


    for(int row=0;row<fam_model->rowCount();row++)
    {
        query.clear();
        query.prepare("INSERT INTO family_data (id_persones,id_family_types,info) VALUES (?,?,?)");
        query.addBindValue(id_persones);
        query.addBindValue(fam_model->data(fam_model->index(row,0),Qt::UserRole).toInt());
        query.addBindValue(fam_model->data(fam_model->index(row,1),Qt::DisplayRole).toString());

        if(!query.exec())
        {
            QString str = query.lastError().databaseText();
            return;
        }
    }
}
