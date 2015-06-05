#ifndef EVENT_OBJECT_H
#define EVENT_OBJECT_H

#include "config_eventsmodel.h"
#include <QObject>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QDebug>

class __EXPORT_EVENTSMODEL EventObject : public QObject
{
    Q_OBJECT
public:
    EventObject(int id_object=0, QString table = "",bool is_source = false, QObject *parent = 0);
    bool isSource();
    QString getTableName();
    int getIdObject();
    QString getTypeObject();
    int getIdEventObjects();
    int getIdTypeEventObject();
    bool insertInDB(int id_event);

    void setTypeObject(QString typeObject);
    void setIdObject(int id_object);
    void setTableName(QString table);
    void setIsSource(bool is_source);
    void setIdTypeEventObject(int id_type_event_object);
    void setIdEventObjects(int id_event_objects);
signals:

public slots:

protected:

    bool is_source;
    int id_object;
    QString table;
    QString typeObject;
    int id_event_objects;
    int id_type_event_object;
};

#endif // EVENT_OBJECT_H
