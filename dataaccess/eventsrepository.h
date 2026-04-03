#ifndef EVENTSREPOSITORY_H
#define EVENTSREPOSITORY_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QVariantList>
#include <QVariantMap>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS EventsRepository : public QObject
{
    Q_OBJECT

public:
    explicit EventsRepository(QObject *parent = nullptr);

    static EventsRepository* qmlSingleton(QQmlEngine*, QJSEngine*);
    static EventsRepository* instance();

    Q_INVOKABLE QVariantList listEvents(const QString &nameFilter = QString(), int limit = 300, int offset = 0);
    Q_INVOKABLE QVariantMap eventDetails(int eventId);

private:
    static EventsRepository *s_instance;
};

#endif // EVENTSREPOSITORY_H
