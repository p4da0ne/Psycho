#include "event.h"

Event::Event(QString name, QString description, int id_status, int id_type_event, QDateTime *start_date, QDateTime *end_date, QList<EventObject *> *objects, Coord *coordinate)
{
    this->id_event = 0;
    this->loadEventStatus();
    this->loadEventTypes();
    this->event_name = name;
    this->event_description = description;
    this->id_status = id_status;
    this->id_type_event = id_type_event;
    mediaModel = new QStandardItemModel();
    if(start_date > 0)
        this->event_start_date = *start_date;
    else
        this->event_start_date = *new QDateTime();

    if(end_date > 0)
        this->event_end_date = *end_date;
    else
        this->event_end_date = *new QDateTime();

    if(objects > 0)
        this->objects = objects;
    else
        this->objects = new QList<EventObject *>;
}

Event::Event(int id_event)
{
    this->loadEventStatus();
    this->loadEventTypes();
    this->objects = new QList<EventObject *>;
    mediaModel = new QStandardItemModel();
    QSqlQuery query;
    this->id_event = id_event;
    if(!query.exec(QString("SELECT * FROM events where id_event = %1").arg(id_event))){
        return;
    }
    int index_id_type_event = query.record().indexOf("id_type_event");
    int index_id_event_status = query.record().indexOf("id_event_status");
    int index_name_event = query.record().indexOf("name_event");
    int index_description_event = query.record().indexOf("description_event");
    int index_resume_event = query.record().indexOf("resume_event");
    int index_time_event_start = query.record().indexOf("time_event_start");
    int index_time_event_end = query.record().indexOf("time_event_end");
    while(query.next()){

        this->event_name = query.value(index_name_event).toString();
        this->event_description = query.value(index_description_event).toString();
        this->id_status = query.value(index_id_event_status).toInt();
        this->id_type_event = query.value(index_id_type_event).toInt();
        this->event_start_date = query.value(index_time_event_start).toDateTime();
        this->event_end_date = query.value(index_time_event_end).toDateTime();
        this->event_resume = query.value(index_resume_event).toString();
    }
    query.clear();
    if(!query.exec(QString("SELECT o.id_event_objects, ty.name_type_event_object, ty.\"table_name\", o.id_object, o.is_events_source, o.id_type_event_object FROM event_objects o, type_event_object ty WHERE o.id_event = %1 AND ty.id_type_event_object = o.id_type_event_object").arg(id_event))){
        return;
    }
    int index_id_event_objects = query.record().indexOf("id_event_objects");
    int index_name_type_event_object = query.record().indexOf("name_type_event_object");
    int index_table_name = query.record().indexOf("table_name");
    int index_id_object = query.record().indexOf("id_object");
    int index_is_events_source = query.record().indexOf("is_events_source");
    int index_id_type_event_object = query.record().indexOf("id_type_event_object");

    while(query.next()){
        EventObject * eventObject = new EventObject();
        eventObject->setIdEventObjects(query.value(index_id_event_objects).toInt());
        eventObject->setIdObject( query.value(index_id_object).toInt());
        eventObject->setTypeObject( query.value(index_name_type_event_object).toString());
        eventObject->setTableName( query.value(index_table_name).toString());
        eventObject->setIsSource( query.value(index_is_events_source).toBool());
        eventObject->setIdTypeEventObject(query.value(index_id_type_event_object).toInt());
        objects->append(eventObject);
    }
    CoordModel *CM = new CoordModel();
    this->setCoordinate(CM->getCoordinates("events",this->id_event,"id_event").at(0));
}

bool Event::setName(QString name){
    if (this->updateEvent("events","name_event",name)){
        this->event_name = name;
        return true;
    }
    else return false;
}

bool Event::setDescription(QString description){
    if (this->updateEvent("events","description_event",description)){
        this->event_description = description;
        return true;
    }
    else return false;
}

bool Event::setStatus(int id_status)
{
    if (this->updateEvent("events","id_event_status",QString::number(id_status))){
        this->id_status = id_status;
        return true;
    }
    else return false;
}

bool Event::setIdTypeEvent(int id_type_event)
{
    if (this->updateEvent("events","id_type_event",QString::number(id_type_event))){
        this->id_type_event = id_type_event;
        return true;
    }
    else return false;
}

bool Event::setStartDate(QDateTime start_date)
{
    if (this->updateEvent("events","time_event_start",start_date.toString("yyyy-MM-dd hh:mm:ss"))){
        qDebug() << start_date.toString("yyyy-MM-dd hh:mm:ss");
        this->event_start_date = start_date;
        qDebug() << "fack time" << this->event_start_date.toString("yyyy-MM-dd hh:mm:ss");
        return true;
    }
    else {
        qDebug() << "fack time";
        return false;
    }
}

bool Event::setEndDate(QDateTime end_date)
{
    if (this->updateEvent("events","time_event_end",end_date.toString("yyyy-MM-dd hh:mm:ss"))){
        qDebug() << end_date.toString("yyyy-MM-dd hh:mm:ss");
        this->event_end_date = end_date;
        qDebug() << "fack time" << this->event_end_date.toString("yyyy-MM-dd hh:mm:ss");
        return true;
    }
    else {
        qDebug() << "fack time";
        return false;
    }
}

void Event::setCoordinate(Coord * coordinate)
{
    CoordModel *cModel;
    if(this->id_event!=0){
        cModel->insertObjectCoord(coordinate , QString("events") , this->id_event);
    }
    this->event_coordinate = coordinate;
}

bool Event::setResume(QString resume)
{
    if (this->updateEvent("events","resume_event",resume)){
        this->event_resume = resume;
        return true;
    }
    else return false;
}

bool Event::setEventObjects(QList<EventObject *> *objects)
{
    this->objects->clear();
    for(int i=0; i< objects->size(); i++){
        if(!this->addEventObject(objects->at(i))){
            return false;
        }
    }
    return true;
}

bool Event::addEventObjects(QList<EventObject *> *objects)
{
    for(int i=0; i< objects->size(); i++){
        if(!this->addEventObject(objects->at(i))){
            return false;
        }
    }
    return true;
}

bool Event::addEventObject(EventObject *object)
{
    if (this->id_event == 0){
        this->objects->append(object);
        return true;
    }else{
        if(object->insertInDB(this->id_event)){
            for(int i=0;i < objects->size(); i++){
                if(objects->at(i)->getIdEventObjects() == object->getIdEventObjects()){
                    objects->removeAt(i);
                }
            }
            this->objects->append(object);
            return true;
        }else{
            return false;
        }
    }
}

bool Event::insertEventToDB()
{
    if(     (this->event_name == "") ||
            (this->id_status == 0) ||
            (this->id_type_event == 0) ||
            (&this->event_start_date == 0) ||
            (this->objects == 0) ||
            (this->event_coordinate == 0) ||
            (this->id_event != 0)){
        return false;
    }
    QSqlQuery query;
    CoordModel *cModel;
    QString str = QString("INSERT INTO events (id_type_event,id_event_status,name_event,description_event,resume_event,time_event_start,time_event_end) VALUES (%1,%2,'%3','%4','%5','%6','%7')  RETURNING id_event")
            .arg(this->id_type_event)
            .arg(this->id_status)
            .arg(this->event_name)
            .arg(this->event_description)
            .arg(this->event_resume)
            .arg(this->event_start_date.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(this->event_end_date.toString("yyyy-MM-dd hh:mm:ss"));
    if(!query.exec(str)){
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        return false;
    }
    while(query.next()){
        this->id_event = query.value(0).toInt();
    }
    query.clear();
    if(!cModel->insertObjectCoord(this->event_coordinate,"events",this->id_event)){
        qDebug() << "Coordinates don't insert. Events roll back transaction.";

        query.exec(QString("DELETE FROM events WHERE id_event = %1").arg(this->id_event));
        this->id_event = 0;
        return false;
    }
    for(int i=0; i < this->objects->size(); i++){
        if(!this->objects->at(i)->insertInDB(this->id_event)){
            qDebug() << "Event objects don't insert. Events roll back transaction.";
            query.exec(QString("DELETE FROM event_objects WHERE id_event = %1").arg(this->id_event));
            query.exec(QString("DELETE FROM coord_events WHERE id_event = %1").arg(this->id_event));
            query.exec(QString("DELETE FROM events WHERE id_event = %1").arg(this->id_event));
            this->id_event = 0;
            return false;
        }
    }
    return true;
}

bool Event::DeleteEvent(int id_event)
{
    QSqlQuery query;
    if(!query.exec(QString("DELETE FROM events WHERE id_event = %1").arg(id_event))){
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

bool Event::DeleteEventMedia(int id_event_media)
{
    QSqlQuery query;
    if(!query.exec(QString("DELETE FROM event_media WHERE id_event_media = %1").arg(id_event_media))){
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

bool Event::DeleteThisEventFromDB()
{
    if(this->id_event == 0){
        qDebug() << "Id event is 0";
        return false;
    }
    QSqlQuery query;
    if(!query.exec(QString("DELETE FROM events WHERE id_event = %1").arg(this->id_event))){
        qDebug() << query.lastError().text();
        return false;
    }
    this->id_event = 0;
    return true;
}

/*!
Открывает медиа файлы из БД средствами ОС
openMediaContent(int id_event_media)
*/
void Event::openMediaContent(QModelIndex index){
    int id_event_media = index.data(34).toInt();
    QSqlQuery query;
    if(!query.exec(QString("SELECT * FROM event_media where id_event_media = %1").arg(id_event_media))){
        qDebug() << query.lastError();
        return;
    }
    int index_name_event_media = query.record().indexOf("filename_media");
    int index_media = query.record().indexOf("media");
    while(query.next()){
        QString file_path=QDir::tempPath() + "/" + query.value(index_name_event_media).toString();
        QFile file(file_path);
        file.open(QIODevice::WriteOnly);
        file.write(query.value(index_media).toByteArray());
        file.close();
        QDesktopServices::openUrl(QUrl(file_path.toUtf8()));
    }
}

void Event::updateMediaEvents(){
    if (this->id_event == 0){
        return;
    }
    QSqlQuery query;
    mediaModel->clear();
    QStandardItem * rootItem = mediaModel->invisibleRootItem();
    query.exec(QString("SELECT em.id_event_media, em.filename_media, em.description, em.id_media_type, mt.type_name FROM event_media em,media_type mt  where mt.id_media_type = em.id_media_type AND id_event = %1").arg(this->id_event));
    int index_name_event_media = query.record().indexOf("filename_media");
    int index_description = query.record().indexOf("description");
    int index_id_event_media = query.record().indexOf("id_event_media");
    int index_type_name = query.record().indexOf("type_name");
    QStringList header;
    header << "Тип" << "Описание";
    mediaModel->setHorizontalHeaderLabels(header);
    while (query.next())
    {
        QList<QStandardItem *> media_items;
        int id_event_media = query.value(index_id_event_media).toInt();
        QStandardItem * item_name = new QStandardItem(query.value(index_type_name).toString());
        QStandardItem * item_description = new QStandardItem(query.value(index_description).toString());
        item_name->setEditable(false);
        item_name->setData("media",33);
        item_name->setData(id_event_media,34);
        item_description->setEditable(false);
        item_description->setData("media",33);
        item_description->setData(id_event_media,34);
        media_items.append(item_name);
        media_items.append(item_description);
        rootItem->appendRow(media_items);
    }
}

int Event::InsertMediaItems(QString path,int idMediaType, QString name_event_media,QString description){
    if(this->id_event == 0){
        return 0;
    }
    MediaInsertThread * MT = new MediaInsertThread();
    MT->id_event = this->id_event;
    MT->path = path;
    MT->idMediaType = idMediaType;
    MT->name_event_media = name_event_media;
    MT->description = description;
    MT->start();
    QMessageBox msgBox;
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Не получается открыть файл. Проверте праильность пути к файлу.");
    msgBox.setStandardButtons(QMessageBox::Ok);

    connect(MT,SIGNAL(BigFile(QString)),&msgBox,SLOT(show()));
    connect(MT,SIGNAL(BigFile(QString)),this,SIGNAL(ErrorMediaContentInsert(QString)));
    connect(MT,SIGNAL(MediaInserted(int)),this,SIGNAL(MediaContentInserted(int)));
    connect(MT,SIGNAL(ErrorMediaInsert(QString)),this,SIGNAL(ErrorMediaContentInsert(QString)));
}

QString Event::getName()
{
    return this->event_name;
}

QString Event::getDescription()
{
    return this->event_description;
}

QString Event::getStatusName()
{
    return events_status.value(this->id_status,"");
}

int Event::getIdStatus()
{
    return this->id_status;
}

QString Event::getResume()
{
    return this->event_resume;
}

QStandardItemModel *Event::getMediaEvents()
{
    return mediaModel;
}

QMap<int, QString> Event::getEventsStatus()
{
    return this->events_status;
}

QMap<int, QMap<QString, int> > Event::getEventsTypes()
{
    return this->events_types;
}

int Event::getIdTypeEvent()
{
    return this->id_type_event;
}

QDateTime * Event::getStartDate()
{
    return &this->event_start_date;
}

QDateTime * Event::getEndDate()
{
    return &this->event_end_date;
}

QList<EventObject *> * Event::getObjects()
{
    return this->objects;
}

Coord *Event::getCoordinate()
{
    return this->event_coordinate;
}

QString Event::getSignCode()
{
    return this->signCode;
}

void Event::loadEventStatus()
{
    QSqlQuery query;
    if(!query.exec("SELECT * FROM event_status")){
        return;
    }
    int index_id_event_status = query.record().indexOf("id_event_status");
    int index_name_event_status = query.record().indexOf("name_event_status");
    while(query.next()){
        events_status.insert(query.value(index_id_event_status).toInt(),query.value(index_name_event_status).toString());
    }
}

void Event::loadEventTypes()
{
    QSqlQuery query;
    if(!query.exec("SELECT * FROM type_event")){
        return;
    }
    int index_id_type_event = query.record().indexOf("id_type_event");
    int index_name_type_event = query.record().indexOf("name_type_event");
    int index_id_sign = query.record().indexOf("id_sign");
    while(query.next()){
        QMap<QString,int> data;
        data.insert(query.value(index_name_type_event).toString(), query.value(index_id_sign).toInt());
        events_types.insert(query.value(index_id_type_event).toInt(),data);
    }
}

bool Event::updateEvent(QString table, QString field, QString set_data)
{
    if (this->id_event == 0){
        qDebug() << "true not update" << table << field << set_data;
        return true;
    }
    QSqlQuery query;
    if(!query.exec(QString("UPDATE %1 SET %2 = '%3' WHERE id_event = %4").arg(table).arg(field).arg(set_data).arg(id_event))){
        qDebug() << query.lastError().text();
        qDebug() << "false update" << table << field << set_data;
        return false;
    }
    qDebug() << "true update" << table << field << set_data;
    return true;
}


void MediaInsertThread::run()
{
    QSqlQuery query;
    query.prepare("INSERT INTO event_media (id_event,id_media_type,filename_media,description, media) VALUES (?,?,?,?,?) RETURNING id_event_media");
    query.addBindValue(id_event);
    query.addBindValue(idMediaType);
    query.addBindValue(name_event_media);
    query.addBindValue(description);
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    if(file.size() > 30000000){
        qDebug() << "big file";
        QString s = "Файл не должен превышать 30 МБ";
        emit BigFile(s);
        return;
    }
    QByteArray ba = file.readAll();
    query.addBindValue(ba,QSql::Binary);

    if(!query.exec()){
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        emit ErrorMediaInsert(query.lastError().text());
        return;
    }

    query.next();
    int id_event_media = query.value(0).toInt();
    query.clear();
    emit MediaInserted(id_event_media);
}
