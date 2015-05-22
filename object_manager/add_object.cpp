#include "add_object.h"
#include <QMessageBox>
#include <QtSql>
#include "ui_add_object.h"

add_object::add_object(int id_region, bool is_parent_ko,QWidget *parent)
	: QDialog(parent),
	 UI(new Ui::add_object)
{
	UI->setupUi(this);
	
	int i=0;
	id_region_1=id_region;
	id_bool_preg=is_parent_ko;
	connect(UI->add_object_pushButton,SIGNAL(clicked()),this,SLOT(add_new_object()));
	connect(UI->cancel_pushButton,SIGNAL(clicked()),this,SLOT(close()));
	setWindowTitle("Добавить регион");
    setWindowIcon(QIcon(":/Resources/planet.png"));
    fill_combobox_type_region(UI->type_region_combo);
//	setWindowIcon(QIcon("./img/database.png"));

}

add_object::~add_object()
{
	delete UI;
}
//========================== комбобокс страны =============================
void add_object::fill_combobox_type_region(QComboBox *box)
{
    box->clear();
    QSqlQuery query;
    box->addItem("-");
    query.exec("SELECT id_type_region,name_type_region FROM type_region ");

    while (query.next()){

        QString name_type = query.value(1).toString();
        int id_type = query.value(0).toInt();
        box->addItem(name_type,id_type);
        }
    query.clear();
}
void add_object::add_new_object(){

    if (UI->type_region_combo->currentIndex() == 0){
       QMessageBox::StandardButton ret;
       ret = QMessageBox::critical (this,"Ошибка",("Выберите тип региона "),QMessageBox::Ok );
    }


	QString name_region=UI->name_region_lineEdit->text();		
    int id_type = UI->type_region_combo->itemData(UI->type_region_combo->currentIndex()).toInt();
	QString description_region=UI->desc_region_textEdit->toPlainText();
	
	if (UI->name_region_lineEdit->text() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите наименование региона "),QMessageBox::Ok);return;}
	else if (UI->desc_region_textEdit->toPlainText() == ""){QMessageBox::StandardButton ret; ret = QMessageBox::warning (this,"Ошибка",("Введите описание региона "),QMessageBox::Ok);return;}
	
		QSqlQuery query;

		if (id_bool_preg == true){
        query.prepare("insert into REGION (name_region,id_type_region,description_region,id_country) values (?,?,?,?)");
		}
		else {
        query.prepare("insert into REGION (name_region,id_type_region,description_region,parent_region) values (?,?,?,?)");
		}

		query.addBindValue(name_region);
        query.addBindValue(id_type);
		query.addBindValue(description_region);
		query.addBindValue(id_region_1);
		
		if(!query.exec()){
		  return;
		}

		 UI->name_region_lineEdit->clear();
         UI->type_region_combo->clear();
		 UI->desc_region_textEdit->clear();
		 close();
}
