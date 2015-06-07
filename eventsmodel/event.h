#ifndef EVENT_H
#define EVENT_H

#include "config_eventsmodel.h"
#include <QObject>
#include <QString>
#include <QStandardItemModel>
#include <QDateTime>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include "../coord/coord.h"
#include "../coord/coordmodel.h"
#include "event_object.h"
#include <saturnSignTypes.h>

class __EXPORT_EVENTSMODEL Event : public QObject
{
    Q_OBJECT
public:
    Event(QString name = "",
          QString description = "",
          int id_status = 0,
          int id_type_event = 0,
          QDateTime * start_date = 0,
          QDateTime * end_date = 0,
          QList<EventObject *> * objects = 0,
          Coord * coordinate = 0);
    Event(int id_event);
    bool setName(QString name);
    bool setDescription(QString description);
    bool setStatus(int id_status);
    bool setIdTypeEvent(int id_type_event);
    bool setStartDate(QDateTime *start_date);
    bool setEndDate(QDateTime *end_date);
    void setCoordinate(Coord * coordinate);
    bool setResume(QString resume);
    bool setEventObjects(QList<EventObject *> * objects);
    bool addEventObject(EventObject * object);
    bool addEventObjects(QList<EventObject *> *objects);



    QString getName();
    QString getDescription();
    QString getStatusName();
    int getIdTypeEvent();
    QString getNameTypeEvent();
    QDateTime *getStartDate();
    QDateTime *getEndDate();
    QList<EventObject *> *getObjects();
    Coord * getCoordinate();
    QString getSignCode();
    int getIdStatus();
    QString getResume();
    //QStandardItemModel * getMediaEvents();


signals:
    
public slots:
    int InsertMediaItems(QString path,int idMediaType, QString name_event_media, QString description = "");
    bool insertEventToDB();
    bool DeleteEvent(int id_event);
    bool DeleteThisEventFromDB();
    void openMediaContent(QModelIndex index);
    QStandardItemModel * getMediaEvents();
    void updateMediaEvents();
    QMap <int,QString> getEventsStatus();
    QMap <int,QMap<QString,int> > getEventsTypes();

protected:
    bool updateEvent(QString table, QString field, QString set_data);



private:
    void loadEventStatus();
    void loadEventTypes();
    QString event_name;
    QString event_description;
    int id_status;
    int id_type_event;
    QDateTime event_start_date;
    QDateTime event_end_date;
    QString event_resume;
    QString signCode;
    Coord * event_coordinate;
    QMap <int,QString> events_status;
    QMap <int,QMap<QString,int> > events_types;
    QList<EventObject *> * objects;
    QStandardItemModel  * mediaModel;
    int id_event;

};

#endif // EVENT_H
