#include <QSqlQuery>
#include <QSqlRecord>
#include <QPixmap>
#include "reports.h"
#include "persones_info.h"
#include "object_manager.h"
#include "ui_persones_info_form.h"

Persones_info::Persones_info(QString type_element,int id_object,QWidget *parent) :
    QDialog(parent),
    UI(new Ui::Persones_info)
{
    UI->setupUi(this);

    id_object_pers = id_object;
    show_info_pers();

//============================================================================

//==================== CONNECT ===========================================

}

Persones_info::~Persones_info()
{
    delete UI;
}
// ====================== информация по персоналии по ID ====================================
void Persones_info::show_info_pers()
{
    UI->type_comboBox->setDisabled(true);
    UI->type_comboBox->setStyleSheet("color: black");
    QSqlQuery query;
    QString str = QString("SELECT id_persones, surname, name, patronumic, rank_persones, \
                          image_persones, birth_date, nationality, birth_place, \
                          persones.id_type_persones,type_persones.name_type_persones \
                          FROM persones, type_persones \
                          WHERE id_persones = %1 \
                          AND persones.id_type_persones = type_persones.id_type_persones").arg(id_object_pers);

    if(!query.exec(str)){
            return;
    }

    QSqlRecord rec = query.record();
    QDate date_d;
    QString f_name,name,o_name,adress,rank_pers,nations;
    int age_pers,id_type_persones,id_person;
    while(query.next()){

        id_person=query.value(rec.indexOf("id_persones")).toInt();
        f_name=query.value(rec.indexOf("surname")).toString();
        name=query.value(rec.indexOf("name")).toString();
        date_d=query.value(rec.indexOf("birth_date")).toDate();
        o_name=query.value(rec.indexOf("patronumic")).toString();
        id_type_persones = query.value(rec.indexOf("id_type_persones")).toInt();
        rank_pers=query.value(rec.indexOf("rank_persones")).toString();
        adress = query.value(rec.indexOf("birth_place")).toString();
        nations = query.value(rec.indexOf("nationality")).toString();

        QPixmap pixmap;
        pixmap.loadFromData(query.value(rec.indexOf("image_persones")).toByteArray() );
        pixmap = pixmap.scaled(200,200,Qt::KeepAspectRatio);

        QString date_ = date_d.toString("dd-MM-yyyy");
        UI->f_LineEdit->setText(f_name);
        UI->name_LineEdit->setText(name);
        UI->o_LineEdit->setText(o_name);
        UI->rankLineEdit->setText(rank_pers);
        UI->date_LineEdit->setText(date_);
        UI->nation_LineEdit->setText(nations);
        UI->home_LineEdit->setText(adress);
        fill_combobox_persones_(UI->type_comboBox,id_type_persones);

        UI->label_foto->setPixmap(pixmap);
        UI->label_foto->setAlignment(Qt::AlignCenter);

    }

}

//======================================================================================
void Persones_info::fill_combobox_persones_(QComboBox *Box,int current_index){

    QSqlQuery query;
    query.exec("SELECT id_type_persones, name_type_persones FROM type_persones");
    int ci_3=0;
    while (query.next())
    {
        QString blok = query.value(1).toString();
        int id_blok=query.value(0).toInt();
        Box->addItem(blok,id_blok);
        if (current_index==id_blok) ci_3=Box->count()-1;
    }
    Box->setCurrentIndex(ci_3);
}
