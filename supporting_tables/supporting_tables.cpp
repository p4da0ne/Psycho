#include "supporting_tables.h"
#include "ui_supporting_tables_form.h"
#if defined Q_OS_WIN
#define kodec QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
#else
#define kodec QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif


SupportingTables::SupportingTables(QDialog *parent)
    : QDialog(parent),
	UI (new Ui::supporting_tables_form)
{
    UI->setupUi(this);
	connect(UI->cancel_button,SIGNAL(clicked()),this,SLOT(close()));
	connect(UI->ok_button,SIGNAL(clicked()),this,SLOT(ok_slot()));
	connect(UI->supp_tables_tree,SIGNAL(itemClicked(QTreeWidgetItem *, int)),this,SLOT(fill_supp_table(QTreeWidgetItem *, int)));
	connect(UI->add_button,SIGNAL(clicked()),this,SLOT(add_supp_table_row()));
	connect(UI->save_button,SIGNAL(clicked()),this,SLOT(save_supp_table_data()));
	connect(UI->delete_button,SIGNAL(clicked()),this,SLOT(del_supp_table_data()));

	//connect(UI->supp_table,SIGNAL(cellClicked(int,int)),this,SLOT(edit_pattern_obj(int,int)));
	QTextCodec *codec = QTextCodec::codecForName("CP1251");
	QTextCodec::setCodecForCStrings(codec);

	init_supporting_tree();
}

SupportingTables::~SupportingTables()
{
	delete UI;
}
//

//======= Формирование списка вспомогательных таблиц =======
void SupportingTables::init_supporting_tree()
{
	UI->supp_tables_tree->setHeaderHidden(true);
	UI->supp_tables_tree->setColumnCount(2);
	UI->supp_tables_tree->hideColumn(1);

	
	
	
	//root_item = add_root("Military ranks","military_rank");
	//UI->supp_tables_tree->setCurrentItem(root_item,0);

	root_item = add_root("СМИ","-");
		add_child(root_item,"Тип СМИ","type_smi");
		add_child(root_item,"Представительство","type_office_smi");
		add_child(root_item,"Способ вещания","type_broadcast_smi");
		add_child(root_item,"Уровень вещания","level_smi");
		add_child(root_item,"Направленность","position_smi");
		add_child(root_item,"Тематика","theme_smi");
		/*add_child(root_item,"Аудитория","auditoria_smi");
		add_child(root_item,"Зона охвата","zona_smi");
		add_child(root_item,"Периодичность вещания","period_smi");*/
	
	root_item = add_root("Население","-");
		ch_item = add_child(root_item,"Профессии","profession");
		add_child(root_item,"Возрастные группы","age");

	root_item = add_root("Организации","-");
		add_child(root_item,"Сфера деятельности","sphere_groups");
		add_child(root_item,"Форма организации","form_groups");
		add_child(root_item,"Информационные органы","propaganda_groups");
		add_child(root_item,"Направленность","trend_groups");

//	root_item = add_root("Подразделения","-");
		

	root_item = add_root("МПО","-");
		add_child(root_item,"Типы МПО(ПсО)","type_mpo_pso");
		//add_child(root_item,"Национальности","nations");
		//add_child(root_item,"Религии","confessions");
	root_item = add_root("Воинские формирования","-");
		add_child(root_item,"Виды воинских формирований ","type_ls");

	root_item = add_root("Регионы","-");
		add_child(root_item,"Типы регионов","type_region");
		add_child(root_item,"Особые условия","type_special_conditions");
		
	root_item = add_root("Персоналии","-");
		add_child(root_item,"Типы персоналий","type_persones");
		

		//root_item = add_root("Организации","-");

    //	root_item = add_root("Страны","country");
		
    //	root_item = add_root("Блоки стран","blok");
		
		root_item = add_root("Воинские звания","military_rank");

		root_item = add_root("Должности","rank");

		root_item = add_root("Национальности","nations");

		root_item = add_root("Религии","confessions");

		root_item = add_root("Языки","language");


	

}

//============ Добавление элемента дерева верхнего уровня =============
QTreeWidgetItem * SupportingTables::add_root(QString name, QString table_name)
{
	QTreeWidgetItem *item = new QTreeWidgetItem(UI->supp_tables_tree);
	item->setText(0,name);
	item->setText(1,table_name);

	UI->supp_tables_tree->addTopLevelItem(item);
	return item;
}

//============ Добавление элемента-потомка  =============
QTreeWidgetItem * SupportingTables::add_child(QTreeWidgetItem *parent, QString name, QString table_name)
{
	QTreeWidgetItem *child_item = new QTreeWidgetItem();
	child_item->setText(0,name);
	child_item->setText(1,table_name);
	parent->addChild(child_item);
	return child_item;
}

//=========== Заполнение таблицы по нажатию на элемент дерева вспомогательных таблиц =============
void SupportingTables::fill_supp_table(QTreeWidgetItem * item, int column)
{
	QString table_name = item->text(1);
	if(table_name == "-") {
		QTreeWidgetItem * child = item->child(0);
		UI->supp_tables_tree->setCurrentItem(child,0);
		UI->supp_tables_tree->setItemSelected(child,true);
		return;
	}
	UI->table_name_label->setText(UI->supp_tables_tree->currentItem()->text(0));
	clear_tableWidget(UI->supp_table);
	QSqlQuery query;
	QString str = QString("SELECT * from %1").arg(table_name);
	if(!query.exec(str)){
		return;
	}
	QSqlRecord rec = query.record();
	QStringList list;
	int col_count = rec.count();
	UI->supp_table->setColumnCount(col_count+1);
	UI->supp_table->setColumnWidth(0,25);
	QTableWidgetItem * h_itm = new QTableWidgetItem("");
	UI->supp_table->setHorizontalHeaderItem(0,h_itm);
	for(int col=1;col<=col_count;col++){
		h_itm = new QTableWidgetItem(rec.fieldName(col-1));
		UI->supp_table->setHorizontalHeaderItem(col,h_itm);
	}
//================ Если выбрана таблица object_class ===============================
	if(table_name == "object_class"){
		UI->supp_table->insertColumn(col_count+1);	
		UI->supp_table->setColumnWidth(col_count+1,25);
		QTableWidgetItem * h = new QTableWidgetItem("");
		UI->supp_table->setHorizontalHeaderItem(col_count+1,h);
	}
//======================================================================
	UI->supp_table->hideColumn(1);
	int row = 0;
	while(query.next()){	
		UI->supp_table->insertRow(row);
		QTableWidgetItem *i = new QTableWidgetItem;
		i->setData(Qt::CheckStateRole, Qt::Unchecked);
		UI->supp_table->setItem(row,0,i);
		for(int col=1;col<=col_count;col++){
			QTableWidgetItem * itm = new QTableWidgetItem((query.value(col-1)).toString());
			//itm->setBackgroundColor(Qt::lightGray);
			itm->setTextColor(Qt::darkBlue);
			UI->supp_table->setItem(row,col,itm);
		}
		//====================
		if(table_name == "object_class"){	 
			QIcon icon(QString("./icons/edit.png"));
			QTableWidgetItem *t_item = new QTableWidgetItem(icon,"",0);
			UI->supp_table->setItem(row,col_count+1,t_item);
		}
		//====================
		row++;
	}
	UI->supp_table->resizeColumnsToContents();
}

//========== Функция очищения таблицы (удаление всех строк и столбцов) ===============
void SupportingTables::clear_tableWidget(QTableWidget *table){
	int count_rows = table->rowCount();
	while(count_rows >= 0){
		table->removeRow(count_rows);
		count_rows--;
	}
	int count_cols = table->columnCount();
	while(count_cols >= 0){
		table->removeColumn(count_cols);
		count_cols--;
	}
}

//========== Функция добавления строки в таблицу supp_table ===============
void  SupportingTables::add_supp_table_row(){
	int row_count = UI->supp_table->rowCount();
	if(!UI->supp_table->item(row_count-1,2)&&row_count>0){
		return;
	}
	UI->supp_table->insertRow(row_count);
	QTableWidgetItem *i = new QTableWidgetItem;
	i->setData(Qt::CheckStateRole, Qt::Unchecked);
	UI->supp_table->setItem(row_count,0,i);
	return;
}

//========== Функция сохранения изменений таблицы supp_table ===============
void SupportingTables::save_supp_table_data(){
 int row_count = UI->supp_table->rowCount();
 int col_count = UI->supp_table->columnCount();
 if(!(UI->supp_table->item(row_count-1,1))){
	int flag = 1;
	for(int i=2;i<col_count;i++){
		if((UI->supp_table->item(row_count-1,i)) == 0){
		 //flag = 1;
		}
		else {
			flag = 0;
		}
	}	 
	if(flag == 1)	return;  // === если добавлена строка, но нет данных 
 }
 QSqlQuery query;
 QString str_ins,str_upd,col_name;
 str_ins = "INSERT INTO ";
		str_ins.append(UI->supp_tables_tree->currentItem()->text(1));
		str_ins.append(" (");
 //===== считывание названий столбцов таблицы (insert) ====
 for(int col=2;col<col_count;col++){
	col_name = UI->supp_table->horizontalHeaderItem(col)->text();
	str_ins.append(col_name);
	str_ins.append(",");
 }
 str_ins.chop(1);
 str_ins.append(") VALUES (");
 str_upd = "UPDATE ";
		str_upd.append(UI->supp_tables_tree->currentItem()->text(1));
		str_upd.append(" SET ");
 for(int i=0;i<row_count;i++)   //=== цикл по строкам таблицы supp_table ===
 {	
		if(!UI->supp_table->item(i,1))   // == insert in supp_table
		{
			QString str_q_ins = str_ins;
			for(int col=2;col<col_count;col++)
			{
				str_q_ins.append(" '");
				if(UI->supp_table->item(i,col))
				{
					str_q_ins.append(UI->supp_table->item(i,col)->text());
				}
				str_q_ins.append("',");
			}
			str_q_ins.chop(1);
			str_q_ins.append(")");
			//=== запрос на вставку === 
			if(!query.exec(str_q_ins))
			{
				return;
			}
			//=======================================
		}
		else // == update supp_table
		{
			QString str_q_upd = str_upd;
			for(int col=2;col<col_count;col++)
			{
				col_name = UI->supp_table->horizontalHeaderItem(col)->text();
				str_q_upd.append(col_name);
				str_q_upd.append("= '");
				if(UI->supp_table->item(i,col))
				{
					str_q_upd.append(UI->supp_table->item(i,col)->text());
				}
				str_q_upd.append("',");
			}
			str_q_upd.chop(1);
			str_q_upd.append(" WHERE ");
			col_name = UI->supp_table->horizontalHeaderItem(1)->text();
			str_q_upd.append(col_name);
			str_q_upd.append("= ");
			str_q_upd.append(UI->supp_table->item(i,1)->text());
			str_q_upd.append(" ");
			//=== запрос на обновление === 
			if(!query.exec(str_q_upd)){
				return;
			}
		}	
 }	//for
QTreeWidgetItem * item;
item = UI->supp_tables_tree->currentItem();
fill_supp_table(item, 0);
}

//======== Удаление выбранных строк таблицы supp_table =============
void SupportingTables::del_supp_table_data()
{

	int row_count = UI->supp_table->rowCount();
	bool fl;
	int f = 0;
	for(int i=0;i<row_count;i++){
		f = f + UI->supp_table->item(i,0)->data(Qt::CheckStateRole).toInt();
	}
		if(f > 0){
  			//================MessageBox===============================
			  QMessageBox msgBox;
			  msgBox.setWindowTitle("Внимание!");
			  msgBox.setText("Вы действительно хотите удалить эту строку?");   
			  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
				 switch (msgBox.exec()) {
				 case QMessageBox::Yes:
					 // yes was clicked
					 break;
				 case QMessageBox::No:
					 return;
					 break;
				 default:
					 return;
					 break;
				 }
		}
		else {
			//================MessageBox===============================
			  QMessageBox msgBox;
			  msgBox.setWindowTitle(tr("Message"));
			  msgBox.setText(tr("No one rows selected!"));   
			  msgBox.setStandardButtons(QMessageBox::Ok);
				 switch (msgBox.exec()) {
				 case QMessageBox::Yes:
					 // yes was clicked
					 return;
					 break;
				 }
		}
 //=========================================================	
	QSqlQuery query;
	int id;
	QString str;
	for(int i=0;i<row_count;i++){
		fl = UI->supp_table->item(i,0)->data(Qt::CheckStateRole).toBool();
		if((fl == true) && (!UI->supp_table->item(i,1))) continue;
		if(fl == true){
		  id = (UI->supp_table->item(i,1)->text()).toInt();
		  str = QString("DELETE FROM %1 WHERE %2 = %3").arg(UI->supp_tables_tree->currentItem()->text(1)).arg(UI->supp_table->horizontalHeaderItem(1)->text()).arg(UI->supp_table->item(i,1)->text());
		  if(!query.exec(str)){
		   return;
		  }
		}
	}
	QTreeWidgetItem * item;
	item = UI->supp_tables_tree->currentItem();
	fill_supp_table(item, 0);
	return;
}

//========= Закрытие формы редактирования вспомогательных таблиц с сохранением данных =============
void SupportingTables::ok_slot(){
	save_supp_table_data();
	close();
}

//================= Создание и открытие формы редактирования типовых объектов, входящих в систему объектов ============
void SupportingTables::edit_pattern_obj(int row,int column){
	QString ttt = UI->supp_tables_tree->currentItem()->text(1);
	if(column!=5 ) {
		return;
	} else if(ttt == "weapon_type"){
		return;
	}
	else {
		int id_object_class = UI->supp_table->item(row,1)->text().toInt();
		QDialog *edit_pattern = new QDialog();
		edit_pattern->setMinimumSize(QSize(600,600));
		edit_pattern->setWindowTitle(tr("Pattern objects"));
		QLabel *header = new QLabel();
		header->setAlignment(Qt::AlignCenter);
		QFont *hed_font = new QFont("Arial",10,QFont::Bold);
		header->setFont(*hed_font);
		id = new QLabel(QString::number(id_object_class));
		id->setVisible(false);
		QSqlQuery query;
		QString str = QString("SELECT class_name FROM object_class WHERE id_object_class = %1").arg(id_object_class);
		if(!query.exec(str)){
			return;
		}
		QSqlRecord rec = query.record();
		while(query.next()){	
			header->setText(query.value(rec.indexOf("class_name")).toString());
		}
		QVBoxLayout *v_lay = new QVBoxLayout();
		v_lay->setAlignment(Qt::AlignTop);
		v_lay->addWidget(header);
		patt_obj_table = new QTableWidget();
		patt_obj_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
		connect(patt_obj_table,SIGNAL(cellClicked(int,int)),this,SLOT(del_object(int,int)));
		create_patt_obj_table(id_object_class);
		v_lay->addWidget(patt_obj_table);
		//=====================================	
		QGroupBox *new_obj = new QGroupBox(tr("Add new pattern object"));
		QLabel *lab1 = new QLabel(tr("Object name:"));
		QLabel *lab2 = new QLabel(tr("Short name:"));
		QLabel *lab3 = new QLabel(tr("Classifikator code:"));
		QLabel *lab4 = new QLabel(tr("Description:"));
		obj_name_edit = new QLineEdit();
		obj_short_name_edit = new QLineEdit();
		classif_code_edit = new QLineEdit();
		descr_obj = new QTextEdit();
		descr_obj->resize(200,100);
		QPushButton *add_obj_button = new QPushButton(tr("Add object"));
		connect(add_obj_button,SIGNAL(clicked()),this,SLOT(add_new_object()));
		QGridLayout *grid = new QGridLayout();
		grid->setAlignment(Qt::AlignTop);
		grid->addWidget(lab1,0,0);
		grid->addWidget(obj_name_edit,0,1);
		grid->addWidget(lab2,1,0);
		grid->addWidget(obj_short_name_edit,1,1);
		grid->addWidget(lab3,2,0);
		grid->addWidget(classif_code_edit,2,1);
		grid->addWidget(lab4,3,0);
		grid->addWidget(descr_obj,3,1);
		grid->addWidget(add_obj_button,4,1);
		new_obj->setLayout(grid);
		v_lay->addWidget(new_obj);
		//===================================
		edit_pattern->setLayout(v_lay);
		if(edit_pattern->exec() == QDialog::Accepted){

		}
		return;
	}
}


//============ Заполнение таблицы типовых объектов данными из БД =======================
void SupportingTables::create_patt_obj_table(int id_object_class)
{
	clear_tableWidget(patt_obj_table);

	patt_obj_table->setColumnCount(6);
	patt_obj_table->hideColumn(0);
	
	QStringList header_list;
	header_list<<""<<tr("Type_name")<<tr("Short_name")<<tr("Description")<<tr("Code")<<"";
	patt_obj_table->setHorizontalHeaderLabels(header_list);

	QSqlQuery query;
	QString str = QString("SELECT * FROM object_type_in_class WHERE id_object_class = %1").arg(id_object_class);
	if(!query.exec(str))
	{
	 return;
	}
	QSqlRecord rec = query.record();
	int id_type,code;
	int row = 0;
	QString type_name,short_name,descr;
	QTableWidgetItem *item;
	while(query.next())
	{	
		id_type = query.value(rec.indexOf("id_type")).toInt();
		type_name = query.value(rec.indexOf("type_name")).toString();
		short_name = query.value(rec.indexOf("short_name")).toString();
		descr = query.value(rec.indexOf("description")).toString();
		code = query.value(rec.indexOf("code")).toInt();

		patt_obj_table->insertRow(row);

		item = new QTableWidgetItem(QString::number(id_type));
		patt_obj_table->setItem(row,0,item);

		item = new QTableWidgetItem(type_name);
		patt_obj_table->setItem(row,1,item);

		item = new QTableWidgetItem(short_name);
		patt_obj_table->setItem(row,2,item);

		item = new QTableWidgetItem(descr);
		patt_obj_table->setItem(row,3,item);

		item = new QTableWidgetItem(QString::number(code));
		patt_obj_table->setItem(row,4,item);

		QIcon icon(QString("./icons/close.png"));
		item = new QTableWidgetItem(icon,"",0);
		patt_obj_table->setItem(row,5,item);
	}	
   row++;
   patt_obj_table->resizeColumnsToContents();
}


//========== Добавление нового типового (типа) объекта отностительно выбранного класса (системы объектов) ==============
void SupportingTables::add_new_object()
{
 QString obj_name,short_name,descr;
 int id_object_class = id->text().toInt();

 obj_name = obj_name_edit->text();
 short_name = obj_short_name_edit->text();
 descr = descr_obj->toPlainText();
 int code = classif_code_edit->text().toInt();

 if(obj_name == "") return;    //== если не введено название объекта, то выход

 QSqlQuery query;
 QString str = QString("INSERT INTO object_type_in_class (id_object_class,type_name,short_name,description,code) VALUES (%1,'%2','%3','%4',%5)").arg(id_object_class).arg(obj_name).arg(short_name).arg(descr).arg(code);
	if(!query.exec(str))
	{
	 return;
	}

 obj_name_edit->clear();
 obj_short_name_edit->clear();
 descr_obj->clear();
 
 create_patt_obj_table(id_object_class);
}


//============== Удаление типового объекта ====================
void SupportingTables::del_object(int row,int col)
{
	if(col!=5)
  {
    return;
  }
  else
  {
  int id_type = patt_obj_table->item(row,0)->text().toInt();

  QSqlQuery query;
	QString str = QString("DELETE FROM object_type_in_class WHERE id_type = %1").arg(id_type);
	if(!query.exec(str))
	{
	 return;
	}
    
	create_patt_obj_table(id->text().toInt());
  }
}
