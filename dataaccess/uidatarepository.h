#ifndef UIDATAREPOSITORY_H
#define UIDATAREPOSITORY_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QVariantList>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS UiDataRepository : public QObject
{
    Q_OBJECT

public:
    explicit UiDataRepository(QObject *parent = nullptr);

    static UiDataRepository* qmlSingleton(QQmlEngine*, QJSEngine*);
    static UiDataRepository* instance();

    Q_INVOKABLE QVariantList buildReferenceTree(
        int objectsLimit = 1500,
        int personnelLimit = 1500,
        int eventsLimit = 1500);
    Q_INVOKABLE QVariantList listMapEvents(int limit = 500);

private:
    static UiDataRepository *s_instance;
};

#endif // UIDATAREPOSITORY_H
