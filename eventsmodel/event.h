#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>
#include "coord.h"

class Event : public QObject
{
    Q_OBJECT
public:
    explicit Event(QObject *parent = 0);
    Event(QString name,
          QString description,
          int id_status,
          int id_type_event,
          QDateTime start_date,
          QDateTime end_date,
          QString type_object,
          int id_object,
          Coord * coordinate);
    void setName(QString name);
    void setDescription(QString description);
    void setStatus(int id_status);
    void setTypeEvent(int id_type_event);
    void setStartDate(QDateTime start_date);
    void setEndDate(QDateTime end_date);
    void setTypeObject(QString type_object);
    void setIdObject(int id_object);
    void setCoordinate(Coord * coordinate);

    QString getName();
    QString getDescription();
    int getStatus();
    int getTypeEvent();
    QDateTime getStartDate();
    QDateTime getEndDate();
    QString getTypeObject();
    int getIdObject();
    Coord * getCoordinate();
    QString getSignCode();
    
signals:
    
public slots:

protected:
    QString event_name = "";
    QString event_description;
    int status;
    int type_event;
    QDateTime event_start_date;
    QDateTime event_end_date;
    QString event_type_object;
    int object;
    QString signCode;
    Coord * event_coordinate;

    
};

#endif // EVENT_H
