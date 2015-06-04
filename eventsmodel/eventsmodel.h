#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include "config_eventsmodel.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFile>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include "event.h"

class __EXPORT_EVENTSMODEL EventsModel : public QStandardItemModel
{
    Q_OBJECT

public:
    EventsModel(QObject *parent = 0);


    void insertEvent(Event *event);
public slots:
    void UpdateModel();
    void UpdateItem(QStandardItem &item);

protected:
    void openMediaContent(int id_event_media);
    QList<QList<QStandardItem *> > appendMediaEvent(int id_event);
    int InsertMediaItems(QString path, int id_event, QString name_event_media,QString description="");
    QList<QStandardItem *> appendObjectEvent(int id_type_event_object, int id_object);

signals:
    void eventInserted();
};

#endif // EVENTSMODEL_H
