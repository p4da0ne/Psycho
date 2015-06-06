#include "eventsmodel.h"

EventsModel::EventsModel(QObject *parent)
    : QStandardItemModel(parent)
{
    this->UpdateModel();
}


/*!
Обновление модели на основании данных в БД (таблица events)
UpdateModel()
*/
void EventsModel::UpdateModel(){
    this->clear();
    QStandardItem * rootItem = this->invisibleRootItem();
    QSqlQuery query;
    query.exec("SELECT e.id_event, e.description_event, e.name_event,e.resume_event,e.time_event_start,e.time_event_end, s.id_event_status, s.name_event_status,ty.name_type_event, si.sign_key FROM events e, event_status s, type_event ty, signs si WHERE e.id_event_status = s.id_event_status AND e.id_type_event = ty.id_type_event AND ty.id_sign = si.id_sign ORDER BY time_event_start");
    int name_event = query.record().indexOf("name_event");
    int time_event_start = query.record().indexOf("time_event_start");
    int time_event_end = query.record().indexOf("time_event_end");
    int description_event = query.record().indexOf("description_event");
    int resume_event = query.record().indexOf("resume_event");
    int id_event = query.record().indexOf("id_event");
    int name_event_status = query.record().indexOf("name_event_status");
    int name_type_event = query.record().indexOf("name_type_event");
    int sign_key = query.record().indexOf("sign_key");
    int id_event_status_index = query.record().indexOf("id_event_status");
    QStringList headerLabels ;
    headerLabels << "Наименование события" << "Статус события" << "Тип события" << "Дата и время начала" << "Дата и время окончания";
    this->setHorizontalHeaderLabels(headerLabels);
    while (query.next())
    {
        QBrush *brush;
        QPixmap ico;
        int id_event_status = query.value(id_event_status_index).toInt();
        switch (id_event_status){
            case 1:
                brush =new QBrush(QColor(255,0,0,50));
                ico.load(":/icons/icons/red.ico");
                break;
            case 2:
                brush =new QBrush(QColor(150,150,150,30));
                ico.load(":/icons/icons/grey.ico");
                break;
            case 3:
                brush =new QBrush(QColor(20,255,20,50));
                ico.load(":/icons/icons/magenta.ico");
                break;
            case 4:
                brush =new QBrush(QColor(218,185,0,70));
                ico.load(":/icons/icons/yellow.ico");
                break;
        }

        QList<QStandardItem *> items;
        QStandardItem * item = new QStandardItem(query.value(name_event).toString());
        item->setData("name_event",33);
        items.append(item);
        QStandardItem * name_event_status_item = new QStandardItem(query.value(name_event_status).toString());
//        name_event_status_item->setData(ico,Qt::DecorationRole);
        name_event_status_item->setEditable(false);
        items.append(name_event_status_item);
        QStandardItem * name_type_event_item = new QStandardItem(query.value(name_type_event).toString());
        name_type_event_item->setEditable(false);
        items.append(name_type_event_item);
//        QStandardItem * description_event_item = new QStandardItem(query.value(description_event).toString());
//        description_event_item->setData("description_event",33);
//        items.append(description_event_item);
//        QStandardItem * resume_event_item = new QStandardItem(query.value(resume_event).toString());
//        resume_event_item->setData("resume_event",33);
//        items.append(resume_event_item);
        QStandardItem * time_event_start_item = new QStandardItem(query.value(time_event_start).toString());
        time_event_start_item->setData("time_event_start",33);
        items.append(time_event_start_item);
        QStandardItem * time_event_end_item = new QStandardItem(query.value(time_event_end).toString());
        time_event_end_item->setData("time_event_end",33);
        items.append(time_event_end_item);
        for(int i=0; i< items.size(); i++){
           items.at(i)->setData(id_event,32);
           items.at(i)->setBackground(*brush);
        }
        rootItem->appendRow(items);
    }
    query.clear();
    this->id_current_event = 0;
}

int EventsModel::InsertMediaItems(QString path, int id_event, QString name_event_media,QString description){
    QSqlQuery query;
    query.prepare("INSERT INTO event_media (id_event,name_event_media,description, media) VALUES (?,?,?) RETURNING id_sign");
    query.addBindValue(id_event);
    query.addBindValue(name_event_media);
    query.addBindValue(description);
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        //================MessageBox===============================
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Warning"));
        msgBox.setText(tr("Can't open the file. Please, check the image filepath."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        switch (msgBox.exec()) {
        case QMessageBox::Ok:
            return 0;
            break;
        }
    }

    QByteArray ba = file.readAll();
    query.addBindValue(ba);

    if(!query.exec())
    {
        QMessageBox::about(0,"Error",query.lastError().text());
    }

    query.next();
    int id_event_media = query.value(0).toInt();
    query.clear();
    return id_event_media;
}

QList<QStandardItem *> EventsModel::appendObjectEvent(int id_type_event_object, int id_object){
    QList<QStandardItem *> items;
    QSqlQuery query;
    query.exec(QString("SELECT * FROM type_event_object where id_type_event_object = %1").arg(id_type_event_object));
    int index_name_type_event_object = query.record().indexOf("name_type_event_object");
    int index_table_name = query.record().indexOf("table_name");
    while (query.next())
    {
        QString table_name= query.value(index_table_name).toString();
        QSqlQuery object_query;
        object_query.exec(QString("SELECT * FROM %1 where id_%1 = %2").arg(table_name).arg(id_object));
        int name = object_query.record().indexOf(QString("name_%1").arg(table_name));
        while (object_query.next())
        {
            items.append(new QStandardItem(query.value(index_name_type_event_object).toString()));
            items.append(new QStandardItem(object_query.value(name).toString()));
        }
    }
    return items;
}

QList< QList<QStandardItem *> > EventsModel::appendMediaEvent(int id_event){
    QList<QList<QStandardItem *> > items;
    QSqlQuery query;
    query.exec(QString("SELECT id_event_media, filename_media, description, id_media_type FROM event_media where id_event = %1").arg(id_event));
    int index_name_event_media = query.record().indexOf("filename_media");
    int index_description = query.record().indexOf("description");
    int index_id_event_media = query.record().indexOf("id_event_media");
    while (query.next())
    {
        QList<QStandardItem *> media_items;
        int id_event_media = query.value(index_id_event_media).toInt();
        QStandardItem * item_name = new QStandardItem(query.value(index_name_event_media).toString());
        QStandardItem * item_description = new QStandardItem(query.value(index_description).toString());
        item_name->setData("media",33);
        item_name->setData(id_event_media,34);
        item_description->setData("media",33);
        item_description->setData(id_event_media,34);
        media_items.append(item_name);
        media_items.append(item_description);
        items.append(media_items);
    }
    return items;
}

/*!
Открывает медиа файлы из БД средствами ОС
openMediaContent(int id_event_media)
*/
void EventsModel::openMediaContent(int id_event_media){
    QSqlQuery query;
    query.exec(QString("SELECT * FROM event_media where id_event_media = %1").arg(id_event_media));
    int index_name_event_media = query.record().indexOf("filename_media");
    int index_media = query.record().indexOf("media");
    while(!query.next()){
        QString file_path=QDir::tempPath() + query.value(index_name_event_media).toString();
        QFile file(file_path);
        file.open(QIODevice::WriteOnly);
        file.write(query.value(index_media).toByteArray());
        file.close();
        QDesktopServices::openUrl(QUrl(file_path));
    }
}

void EventsModel::UpdateItem(QStandardItem &item){

}

void EventsModel::rowClicked(const QModelIndex &index){
    int id_event = index.data(32).toInt();
    if(id_event == this->id_current_event){
        return;
    }
    this->id_current_event = id_event;
}

void EventsModel::insertEvent(Event * event){
event->insertEventToDB();
}

Event * EventsModel::getEvent(int id_event){
    return new Event(id_event);
}
