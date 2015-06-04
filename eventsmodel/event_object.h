#ifndef EVENT_OBJECT_H
#define EVENT_OBJECT_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class EventObject : public QObject
{
    Q_OBJECT
public:
    explicit EventObject(int id_object=0, QString table = "",bool is_source = false, QObject *parent = 0);
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

signals:

public slots:

protected:
    void setIdEventObjects(int id_event_objects);
    bool is_source;
    int id_object;
    QString table;
    QString typeObject;
    int id_event_objects;
    int id_type_event_object;
};

#endif // EVENT_OBJECT_H
