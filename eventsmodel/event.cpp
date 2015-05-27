#include "event.h"

Event::Event(QObject *parent) :
    QObject(parent)
{
}

void Event::setName(QString name)
{
    this->event_name = name;
}

void Event::setDescription(QString description)
{
    this->event_description = description;
}

void Event::setStatus(int id_status)
{
    this->status = id_status;
}

void Event::setTypeEvent(int id_type_event)
{
    this->type_event = id_type_event;
}

void Event::setStartDate(QDateTime start_date)
{
    this->event_start_date = start_date;
}

void Event::setEndDate(QDateTime end_date)
{
    this->event_end_date = end_date;
}

void Event::setTypeObject(QString type_object)
{
    this->event_type_object = type_object;
}

void Event::setIdObject(int id_object)
{
    this->object = id_object;
}

void Event::setCoordinate(Coord * coordinate)
{
    this->event_coordinate = coordinate;
}

QString Event::getName()
{
    return this->event_name;
}

QString Event::getDescription()
{
    return this->event_description;
}

int Event::getStatus()
{
    return this->status;
}

int Event::getTypeEvent()
{
    return this->type_event;
}

QDateTime Event::getStartDate()
{
    return this->event_start_date;
}

QDateTime Event::getEndDate()
{
    return this->event_end_date;
}

QString Event::getTypeObject()
{
    return this->event_type_object;
}

int Event::getIdObject()
{
    return this->object;
}

Coord *Event::getCoordinate()
{
    return this->event_coordinate;
}

QString Event::getSignCode()
{
    return this->signCode;
}


