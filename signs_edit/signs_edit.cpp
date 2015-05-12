#include "signs_edit.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QToolButton>

SignsEdit::SignsEdit(QDialog *parent, Qt::WFlags flags)
    : QDialog(parent, flags),
      UI (new Ui::signs_edit_form)
{
    UI->setupUi(this);
    show_object_types();
    connect(UI->close_button,SIGNAL(clicked()),this,SLOT(close()));
    connect(UI->object_types_treeView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(show_signs_table(const QModelIndex &)));
    connect(UI->add_sign_button,SIGNAL(clicked()),this,SLOT(add_new_sign()));
    connect(UI->object_signs_table,SIGNAL(cellClicked(int,int)),this,SLOT(delete_sign(int, int)));
    connect(UI->save_changes_button,SIGNAL(clicked()),this,SLOT(save_changes()));
}

SignsEdit::~SignsEdit(){
    delete UI;
}

/*!
  Построение дерева типов объектов поражения
  void SignsEdit::show_object_types()
  */
void SignsEdit::show_object_types(){
    QString str_query;
    model = new QStandardItemModel(this);
    parentItem = model->invisibleRootItem();

    // Add type from type_ls table
    QStandardItem *type_ls_item = new QStandardItem(tr ("type_ls"));
    type_ls_item->setData("type_ls",Qt::UserRole);
    parentItem->appendRow(type_ls_item);
    str_query="SELECT id_sign, name_type_ls, id_type_ls FROM type_ls order by id_type_ls";
    this->add_type_to_model(str_query,type_ls_item);

    // Add type from type_mpo_pso table
    QStandardItem *type_mpo_pso_item = new QStandardItem(tr ("type_mpo_pso"));
    type_mpo_pso_item->setData("type_mpo_pso",Qt::UserRole);
    parentItem->appendRow(type_mpo_pso_item);
    str_query="SELECT id_sign, name_type_mpo_pso, id_type_mpo_pso FROM type_mpo_pso order by id_type_mpo_pso";
    this->add_type_to_model(str_query,type_mpo_pso_item);

    // Add type from type_persones table
    QStandardItem *type_persones_item = new QStandardItem(tr ("type_persones"));
    type_persones_item->setData("type_persones",Qt::UserRole);
    parentItem->appendRow(type_persones_item);
    str_query="SELECT id_sign, name_type_persones, id_type_persones FROM type_persones order by id_type_persones";
    this->add_type_to_model(str_query,type_persones_item);

    // Add type from type_special_conditions table
    QStandardItem *type_special_conditions_item = new QStandardItem(tr ("type_special_conditions"));
    type_special_conditions_item->setData("type_special_conditions",Qt::UserRole);
    parentItem->appendRow(type_special_conditions_item);
    str_query="SELECT id_sign, name_type_special_conditions id_type_special_conditions FROM type_special_conditions order by id_type_special_conditions";
    this->add_type_to_model(str_query,type_special_conditions_item);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Types of objects"));
    UI->object_types_treeView->setModel(model);
}

/*!
Отображение информации о знаках выбранного типа объектов
void SignsEdit::show_signs_table(const QModelIndex &index)
*/
void SignsEdit::show_signs_table(const QModelIndex &index){
    //---------- Обработка нажатия на элемент дерева типов объектов поражения -----------
    int id_type = index.data(Qt::UserRole).toInt();

    QString type_name = index.data(Qt::DisplayRole).toString();

    UI->object_typename_label->setText(type_name);

    create_signs_table(index);
    //------------------------------------------------
}

/*!
Построение таблицы знаков объектов
void SignsEdit::create_signs_table(const QModelIndex &index)
*/
void SignsEdit::create_signs_table(const QModelIndex &index)
{
    if(index.data(Qt::UserRole).type() == QVariant::String){
        return;
    }
    clear_tableWidget(UI->object_signs_table);
    UI->object_signs_table->setColumnCount(5);
    UI->object_signs_table->hideColumn(0);
    QStringList h_list;
    h_list << "" << tr("Sign") << tr("Code") << tr("Sign name") <<tr("Delete");
    UI->object_signs_table->setHorizontalHeaderLabels(h_list);
    //ЗАпрос на выдачу id_sign из выбранной таблицы
    QString table_type=index.parent().data(Qt::UserRole).toString();
    QSqlQuery query,query_sign;
    QString str = QString("select id_%1, name_%1, id_sign from %1 where id_%1 = %2").arg(table_type).arg(index.data(Qt::UserRole).toInt());
    if(!query_sign.exec(str)){
        return;
    }
    while(query_sign.next())
    {
        int id_sign = query.value(2).toInt();
        query.clear();
        str = QString("select id_sign, sign_name, sign_key, sign_picture from signs where id_sign = %1").arg(id_sign);
        if(!query.exec(str))
        {
            return;
        }
        QSqlRecord rec = query.record();
        int i = 0;

        while(query.next())
        {
            UI->object_signs_table->insertRow(i);
            UI->object_signs_table->setRowHeight(i,80);

            QString id_sign = query.value(rec.indexOf("id_sign")).toString();
            QTableWidgetItem *item = new QTableWidgetItem(id_sign);
            UI->object_signs_table->setItem(i,0,item);

            QPixmap pixmap;
            pixmap.loadFromData( query.value(rec.indexOf("sign_picture")).toByteArray() );
            QTableWidgetItem *foto_item = new QTableWidgetItem(QIcon(pixmap),"");
            UI->object_signs_table->setItem(i,1,foto_item);
            UI->object_signs_table->setIconSize(QSize(60,60));

            item = new QTableWidgetItem(query.value(rec.indexOf("sign_key")).toString());
            UI->object_signs_table->setItem(i,2,item);

            item = new QTableWidgetItem(query.value(rec.indexOf("sign_name")).toString());
            UI->object_signs_table->setItem(i,3,item);

            QIcon icon(QString("./icons/close.png"));
            item = new QTableWidgetItem(icon,"",0);
            UI->object_signs_table->setItem(i,4,item);
            i++;
        }
    }
    UI->object_signs_table->resizeColumnsToContents();
    UI->object_signs_table->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
}


//======= Добавление знака для типа объекта ========
void SignsEdit::add_new_sign()
{
    QModelIndex currentIndex=UI->object_types_treeView->currentIndex();
    QString table_type;
    if (currentIndex.data(Qt::UserRole).isNull()) {
        QMessageBox msgBox;
        msgBox.setText("Pleas, select one of type.");
        msgBox.exec();
        return;
    }
    if (currentIndex.data(Qt::UserRole).type() == QVariant::String){
        QMessageBox msgBox;
        msgBox.setText("Pleas, select one of type.");
        msgBox.exec();
        return;
    }else{
        table_type=UI->object_types_treeView->currentIndex().parent().data(Qt::UserRole).toString();
    }
    add_sign = new QDialog();
    add_sign->setMinimumSize(400,200);
    add_sign->setWindowTitle(tr("Add new sign"));

    path_lab = new QLabel(tr("Sign filepath:"));
    path_lab->setMinimumWidth(100);
    sign_code_lab = new QLabel(tr("Code:"));
    sign_code_lab->setMinimumWidth(100);
    sign_name_lab = new QLabel(tr("Name:"));
    sign_name_lab->setMinimumWidth(100);

    sign_filepath_edit = new QLineEdit();
    sign_code_edit = new QLineEdit();
    sign_name_edit = new QLineEdit();

    path_button = new QToolButton();
    path_button->setText("...");
    connect(path_button,SIGNAL(clicked()),this,SLOT(get_path()));

    ok_button = new QPushButton(tr("OK"));
    connect(ok_button,SIGNAL(clicked()),add_sign,SLOT(accept()));
    cancel_button = new QPushButton(tr("Cancel"));
    connect(cancel_button,SIGNAL(clicked()),add_sign,SLOT(close()));

    QHBoxLayout *buttons_layout = new QHBoxLayout();
    buttons_layout->addWidget(ok_button);
    buttons_layout->addStretch();
    buttons_layout->addWidget(cancel_button);

    QHBoxLayout *path_layout = new QHBoxLayout();
    path_layout->addWidget(path_lab);
    path_layout->addWidget(sign_filepath_edit);
    path_layout->addWidget(path_button);

    QHBoxLayout *code_layout = new QHBoxLayout();
    code_layout->addWidget(sign_code_lab);
    code_layout->addWidget(sign_code_edit);

    QHBoxLayout *name_layout = new QHBoxLayout();
    name_layout->addWidget(sign_name_lab);
    name_layout->addWidget(sign_name_edit);

    QVBoxLayout *v_layout = new QVBoxLayout();
    v_layout->addLayout(path_layout);
    v_layout->addLayout(code_layout);
    v_layout->addLayout(name_layout);
    v_layout->addStretch();
    v_layout->addLayout(buttons_layout);


    add_sign->setLayout(v_layout);

    if(add_sign->exec() == QDialog::Accepted)
    {
        if((sign_filepath_edit->text() == "") || (sign_code_edit->text() == "") || (sign_name_edit->text() == "")) return;

        //------ По кнопке ОК добавление в БД нового знака для типа объекта --------
        QSqlQuery query;
        query.prepare("INSERT INTO signs (sign_key,sign_name,sign_picture) VALUES (?,?,?)");
        query.addBindValue(sign_code_edit->text());
        query.addBindValue(sign_name_edit->text());

        QFile file(sign_filepath_edit->text());
        if(!file.open(QIODevice::ReadOnly))
        {
            //================MessageBox===============================
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(tr("Can't open the file. Please, check the image filepath."));
            msgBox.setStandardButtons(QMessageBox::Yes);
            switch (msgBox.exec()) {
            case QMessageBox::Yes:
                return;
                break;
            }
        }

        QByteArray ba = file.readAll();
        query.addBindValue(ba);

        if(!query.exec())
        {
            QString s = query.lastError().text();
        }
        int id_sign = query.lastInsertId().toInt();
        QString str_query=QString("UPDATE %1 SET id_sign = %3 WHERE id_%1 = %2").arg(table_type).arg(currentIndex.data(Qt::UserRole).toInt()).arg(id_sign);
        if(!query.exec(str_query)){
            QString s = query.lastError().text();
            QMessageBox msgBox;
            msgBox.setText(s);
            msgBox.exec();
            return;
        }
        create_signs_table(UI->object_types_treeView->currentIndex());
        return;
    }

    return;
}


//========== Функция очищения таблицы (удаление всех строк и столбцов) ===============
void SignsEdit::clear_tableWidget(QTableWidget *table)
{
    int count_rows = table->rowCount();
    while(count_rows >= 0)
    {
        table->removeRow(count_rows);
        count_rows--;
    }

    int count_cols = table->columnCount();
    while(count_cols >= 0)
    {
        table->removeColumn(count_cols);
        count_cols--;
    }
}


//====== Получение пути файла изображения знака объекта =============
void SignsEdit::get_path()
{
    QFileDialog *file_dlg = new QFileDialog(add_sign);

    QString filepath =  file_dlg->getOpenFileName(this,
                                                  tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    sign_filepath_edit->setText(filepath);
    add_sign->raise();

}


//=========== Удаление знака объекта ======
void SignsEdit::delete_sign(int row, int column)
{

    if(column != 4)return;
    //================MessageBox===============================
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Warning"));
    msgBox.setText(tr("Do you want to delete the sign?"));
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

    QSqlQuery query;

    int id_sign= UI->object_signs_table->item(row,0)->text().toInt();

    QString str = QString("DELETE FROM signs WHERE id_sign = %1").arg(id_sign);
    if(!query.exec(str))
    {
        return;
    }
    create_signs_table(UI->object_types_treeView->currentIndex());
    return;
}

//======== Сохранение изменений после редактирования знаков ======
void SignsEdit::save_changes()
{
    int row_count = UI->object_signs_table->rowCount();

    QSqlQuery query;

    for(int i=0;i<row_count;i++)
    {
        query.prepare("UPDATE signs SET sign_key = ?, sign_name = ? WHERE id_sign = ?");
        query.addBindValue(UI->object_signs_table->item(i,2)->text());
        query.addBindValue(UI->object_signs_table->item(i,3)->text());
        query.addBindValue(UI->object_signs_table->item(i,0)->text().toInt());

        if(!query.exec())
        {
            QString err = query.lastError().text();
            return;
        }
    }

    //================MessageBox===============================
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Message"));
    msgBox.setText(tr("All changes are succesfully saved!"));
    msgBox.setStandardButtons(QMessageBox::Yes);
    switch (msgBox.exec()) {
    case QMessageBox::Yes:
        // yes was clicked
        break;
    default:
        return;
        break;
    }

}

/*!
Добавление одного типа объекта в модель типов
void SignsEdit::add_type_to_model(QString str_query, QStandardItem *parent)
@str_query - строка запроса первый возвращаемый параметр - id типа, второй - имя
@*parent - родительский узел (содержит название подгруппы типов)
*/
void SignsEdit::add_type_to_model(QString str_query, QStandardItem *parent)
{
    QSqlQuery query;
    query.exec(str_query);
    while (query.next())
    {
        QStandardItem *item = new QStandardItem(query.value(1).toString());
        int id_obj_type=query.value(2).toInt();
        QString data_type_obj = QString::number(id_obj_type);
        item->setData(data_type_obj,Qt::UserRole);
        parent->appendRow(item);
    }
    query.clear();
}
