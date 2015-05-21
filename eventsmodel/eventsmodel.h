#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include "config_eventsmodel.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStandardItemModel>
#include <QStandardItem>

class __EXPORT_EVENTSMODEL EventsModel : public QStandardItemModel
{
    Q_OBJECT

public:
    EventsModel(QObject *parent = 0);
    void UpdateModel();
protected:
    QList<QList<QStandardItem *> > appendMediaEvent(int id_event);
    QStandardItem * InsertMediaItems(QStandardItem *parent, QString name, int id_event, QVariant data = 0, int role = Qt::UserRole + 1);
    QList<QStandardItem *> appendObjectEvent(int id_type_event_object, int id_object);
};

#endif // EVENTSMODEL_H
