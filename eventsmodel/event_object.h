#ifndef EVENT_OBJECT_H
#define EVENT_OBJECT_H

#include <QObject>

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

    void setTypeObject(QString typeObject);
    void setIdObject(int id_object);
    void setTableName(QString table);
    void setIsSource(bool is_source);
    void setIdEventObjects(int id_event_objects);
signals:

public slots:

protected:
    bool is_source;
    int id_object;
    QString table;
    QString typeObject;
    int id_event_objects;
};

#endif // EVENT_OBJECT_H
