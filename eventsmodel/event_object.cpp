#include "event_object.h"

EventObject::EventObject(int id_object, QString table, bool is_source, QObject *parent) :
    QObject(parent)
{
    this->id_object = id_object;
    this->table = table;
    this->is_source = is_source;
}

bool EventObject::isSource()
{
    return this->is_source;
}

QString EventObject::getTableName()
{
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
