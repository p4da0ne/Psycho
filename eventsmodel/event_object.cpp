#include "event_object.h"
#include <QVariant>

EventObject::EventObject(int id_object, QString table, bool is_source, QObject *parent) :
    QObject(parent){
    this->id_event_objects = 0;
    this->id_object = id_object;
    this->table = table;
    this->is_source = is_source;
}

EventObject::EventObject(int id_object, int IdTypeEventObject, bool is_source, QObject *parent):
    QObject(parent)
{
    this->id_event_objects = 0;
    this->id_object = id_object;
    this->id_type_event_object = IdTypeEventObject;
    this->is_source = is_source;
}

bool EventObject::isSource(){
    return this->is_source;
}

QString EventObject::getTableName(){
    return this->table;
}

int EventObject::getIdObject()
{
    return this->id_object;
}

QString EventObject::getTypeObject()
{
    return this->typeObject;
}

int EventObject::getIdEventObjects()
{
    return this->id_event_objects;
}

int EventObject::getIdTypeEventObject()
{
    return this->id_type_event_object;
}

bool EventObject::insertInDB(int id_event)
{
    if(this->id_event_objects != 0){
        qDebug() << "Don't new object id_event_objects != 0";
        return false;
    }
    if(this->id_object == 0){
        qDebug() << "id_object can't be equal 0";
        return false;
    }
    if(this->id_type_event_object == 0){
        qDebug() << "id_type_event_object can't be equal 0";
        return false;
    }
    QSqlQuery query;
    QString str = QString("SELECT * FROM event_objects where id_event = %1 AND is_events_source = '%2'").arg(id_event).arg(this->isSource());
    if(!query.exec(str)){
        qDebug() << query.lastError().text();
        return false;
    }
    if(query.size() > 0){
        str = QString("UPDATE event_objects SET id_type_event_object = %1 , id_object = %2 WHERE id_event = %3 AND is_events_source = '%4'")
                .arg(this->getIdTypeEventObject())
                .arg(this->getIdObject())
                .arg(id_event)
                .arg(this->isSource());

    }else{
        str = QString("INSERT INTO event_objects (id_event, id_type_event_object, id_object, is_events_source) VALUES (%1,%2,%3,'%4')  RETURNING id_event_objects")
                .arg(id_event)
                .arg(this->getIdTypeEventObject())
                .arg(this->getIdObject())
                .arg(this->isSource());
    }
    if (!query.exec(str)){
        qDebug() << query.lastError().text();
        return false;
    }
    while(query.next()){
        this->id_event_objects = query.value(0).toInt();
    }
    query.clear();
    return true;
}

void EventObject::setTypeObject(QString typeObject)
{
    this->typeObject = typeObject;
}

void EventObject::setIdObject(int id_object)
{
    this->id_object = id_object;
}

void EventObject::setTableName(QString table)
{
    this->table = table;
}

void EventObject::setIsSource(bool is_source)
{
    this->is_source = is_source;
}

void EventObject::setIdEventObjects(int id_event_objects)
{
    this->id_event_objects = id_event_objects;
}

void EventObject::setIdTypeEventObject(int id_type)
{
    this->id_type_event_object = id_type;
}
