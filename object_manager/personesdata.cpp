#include "personesdata.h"
#include <QSqlQuery>
#include "ui_personesdata_form.h"

PersonesData::PersonesData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonesData)
{
    ui->setupUi(this);


//===================== размеры полей и тд =========================================================
//    ui->vzv_tableWidget->setColumnCount(5);
//    ui->vzv_tableWidget->hideColumn(1);
//  //  ui->vzv_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->vzv_tableWidget->setColumnWidth(0,50);
//    ui->vzv_tableWidget->setColumnWidth(2,350);
//    ui->vzv_tableWidget->setColumnWidth(3,350);
//    ui->vzv_tableWidget->setColumnWidth(4,50);
//    QStringList header_list;
//    header_list<<" "<<" "<<"Воинские звания \n (специальные звания, \n классные чины )"<<"Правовой акт о присвоении \n воинского звания \n (специального звания, классного чина), \n его дата и номер"<<" ";
//    ui->vzv_tableWidget->setHorizontalHeaderLabels(header_list);


//    ui->educ_tableWidget->setColumnCount(6);
//    ui->educ_tableWidget->hideColumn(1);
//  //  ui->educ_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->educ_tableWidget->setColumnWidth(0,50);
//    ui->educ_tableWidget->setColumnWidth(2,300);
//    ui->educ_tableWidget->setColumnWidth(3,300);
//    ui->educ_tableWidget->setColumnWidth(4,300);
//    ui->educ_tableWidget->setColumnWidth(5,50);
//    QStringList header_list_table_educ;
//    header_list_table_educ<<" "<<" "<<"Наименования учебных заведений"<<"Год окончания"<<"Наименования факультетов"<<" ";
//    ui->educ_tableWidget->setHorizontalHeaderLabels(header_list_table_educ);


    fill_combobox_persones(ui->type_person_comboBox);
    //======================================================================================================
}

PersonesData::~PersonesData()
{
    delete ui;
}

void PersonesData::fill_combobox_persones(QComboBox *box)
{
    box->clear();
    QSqlQuery query;
    box->addItem("-");
    query.exec("SELECT id_type_persones, name_type_persones FROM type_persones");
    while (query.next())
    {
        QString name_type = query.value(1).toString();
        int id_type = query.value(0).toInt();
        box->addItem(name_type,id_type);
    }
    query.clear();
}
