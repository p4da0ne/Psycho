#ifndef OBJECTDETAILSREPOSITORY_H
#define OBJECTDETAILSREPOSITORY_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QVariantMap>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS ObjectDetailsRepository : public QObject
{
    Q_OBJECT

public:
    explicit ObjectDetailsRepository(QObject *parent = nullptr);

    static ObjectDetailsRepository* qmlSingleton(QQmlEngine*, QJSEngine*);
    static ObjectDetailsRepository* instance();

    Q_INVOKABLE QVariantMap objectSummary(int objectType, int objectId);
    Q_INVOKABLE QVariantMap objectDetails(int objectType, int objectId);

private:
    static ObjectDetailsRepository *s_instance;
};

#endif // OBJECTDETAILSREPOSITORY_H
