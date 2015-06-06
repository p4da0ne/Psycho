#include "personesdata.h"
#include <QSqlQuery>
#include <QFile>
#include <QFileDialog>
#include <QSqlError>
#include <QMessageBox>
#include "ui_personesdata_form.h"


PersonesData::PersonesData(QString type_element,int id_object,QWidget *parent) :
    QDialog(parent),
    UI(new Ui::PersonesData)
{
    UI->setupUi(this);


    id_object_pers = id_object;
    type_elem_pers = type_element;


 //===================== размеры полей и тд =========================================================

//    header_list<<" "<<" "<<"Воинские звания \n (специальные звания, \n классные чины )"<<"Правовой акт о присвоении \n воинского звания \n (специального звания, классного чина), \n его дата и номер"<<" ";

//    header_list_table_educ<<" "<<" "<<"Наименования учебных заведений"<<"Год окончания"<<"Наименования факультетов"<<" ";
//================================== CONNECT =======================================================

    connect(UI->save_all_button,SIGNAL(clicked()),this,SLOT(save_persones()));
    connect(UI->path_foto_toolButton,SIGNAL(clicked()),this,SLOT(open_file()));
//==================================================================================================
    fill_combobox_persones(UI->type_person_comboBox);
//==================================================================================================

    QDate dateToday = QDate::currentDate();
    UI->hb_dateEdit->setDate(dateToday);
    UI->date_pass_dateEdit->setDate(dateToday);
    UI->date_udo_lich_dateEdit->setDate(dateToday);
    UI->zvan_nauka_dateEdit->setDate(dateToday);
    UI->step_nauka_dateEdit->setDate(dateToday);
    UI->prinyl_dateEdit->setDate(dateToday);
    UI->prizvan_dateEdit->setDate(dateToday);
    UI->prizvan_2_dateEdit->setDate(dateToday);
    UI->wife_b_dateEdit->setDate(dateToday);
    UI->brak_dateEdit->setDate(dateToday);

    UI->itog_dateEdit->setDate(dateToday);


}

PersonesData::~PersonesData()
{
    delete UI;
}

void PersonesData::save_persones()
{
    int id_type_pers_=UI->type_person_comboBox->itemData(UI->type_person_comboBox->currentIndex()).toInt();
    QString name_persones = UI->last_name_lineEdit->text();
  //  QString desc_pers=textEdit_propa->toPlainText();
  //  int counte_age = lineEdit_counte_ls->text().toInt();
   // QString contact = lineEdit_counte_ls_bd->text();
  //  QString rank_pers = lineEdit_rank->text();
  //  float aut = lineEdit_aut->text().toFloat();
  //  bool opossition = checkbox_enemy->isChecked();

    QMap<QString,QString> map;

        if (id_object_pers<1) return;
        map.clear();
        map.insert("name_persones",name_persones);
    //	map.insert("age_persones",QString::number(counte_age));
    //	map.insert("authority_persones",QString::number(aut));
    //	map.insert("contact_persones",contact);
    //	map.insert("rank_persones",rank_pers);

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

   //   map.insert("opposition_persones",QString::number(opossition));
    //	map.insert("description_persones",desc_pers);
        map.insert("id_type_persones",QString::number(id_type_pers_));


        int id_rezult=insert_in_table("persones",map,"id_persones");

//======================= фото персоны ===================================
        QFile file(UI->path_foto_lineEdit->text());
        if(!file.open(QIODevice::ReadOnly))
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

        QSqlQuery query;

        query.prepare("UPDATE persones SET image_persones = ? WHERE id_persones = ?");

        QByteArray image_persones = file.readAll();
        query.addBindValue(image_persones);
        query.addBindValue(id_rezult);
        if(!query.exec())
        {
            QString s = query.lastError().text();
        }

        if(id_rezult>0){
            this->done(id_rezult);
        }else{
            this->done(0);
        }

}
//======================= выбор типа персоналии в комбо ========================================
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
        bool res=query.exec(QString("insert into %1 (%2) values (%3) returning %4").arg(table).arg(table_columns).arg(columns_values).arg(id));
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
}
//=========================== путь к файлу с фото ==================================
void PersonesData::open_file(){
    QString fileName = QFileDialog::getOpenFileName(this, "Выбор фотографии", "",
                                                    "Images (*.jpg *.png)");
    if (fileName.isEmpty()) return;
    UI->path_foto_lineEdit->setText(fileName);
    this->raise();
}
