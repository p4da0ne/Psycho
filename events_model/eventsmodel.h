#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include "events_model_global.h"
 #include <QSqlQueryModel>

class EVENTS_MODELSHARED_EXPORT EventsModel: public QSqlQueryModel {
Q_OBJECT
public:
    EventsModel(QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role);

    private:
        bool setFirstName(int personId, const QString &firstName);
        bool setLastName(int personId, const QString &lastName);
        void refresh();
};

#endif // EVENTSMODEL_H
