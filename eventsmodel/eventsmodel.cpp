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
    query.exec("SELECT * FROM events ORDER BY time_event_start");
    int name_event = query.record().indexOf("name_event");
    int time_event_start = query.record().indexOf("time_event_start");
    int time_event_end = query.record().indexOf("time_event_end");
    int description_event = query.record().indexOf("description_event");
    int id_type_event_object = query.record().indexOf("id_type_event_object");
    int id_object = query.record().indexOf("id_object");
    int id_event = query.record().indexOf("id_event");
    while (query.next())
    {
        QList<QStandardItem *> items;
        QStandardItem * item = new QStandardItem(query.value(name_event).toString());
        QList<QList<QStandardItem *> > media_items;
        media_items = appendMediaEvent(query.value(id_event).toInt());
        for (int i = 0; i < media_items.size(); ++i) {
            item->appendRow(media_items.at(i));
        }
        items.append(item);
        items.append(new QStandardItem(query.value(name_event).toString()));
        items.append(new QStandardItem(query.value(time_event_start).toString()));
        items.append(new QStandardItem(query.value(time_event_end).toString()));
        items.append(new QStandardItem(query.value(description_event).toString()));
        items.append(this->appendObjectEvent(query.value(id_type_event_object).toInt(),query.value(id_object).toInt()));
        rootItem->appendRow(items);
    }
    query.clear();
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
    query.exec(QString("SELECT id_event_media, name_event_media, description FROM event_media where id_event = %1").arg(id_event));
    int index_name_event_media = query.record().indexOf("name_event_media");
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
    int index_name_event_media = query.record().indexOf("name_event_media");
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
