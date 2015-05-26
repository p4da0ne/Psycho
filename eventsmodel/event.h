#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QString>
#include <QDateTime>

class Event : public QObject
{
    Q_OBJECT
public:
    explicit Event(QObject *parent = 0);
    Event();
    
signals:
    
public slots:

protected:
    QString event_name;
    QString description;
    int id_status;
    int id_type_event;
    QDateTime start_date;
    QDateTime end_date;
    QString type_object;
    int id_object;
    
};

#endif // EVENT_H
