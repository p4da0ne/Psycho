#ifndef PERSONNELREPOSITORY_H
#define PERSONNELREPOSITORY_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QVariantList>
#include <QVariantMap>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS PersonnelRepository : public QObject
{
    Q_OBJECT

public:
    explicit PersonnelRepository(QObject *parent = nullptr);

    static PersonnelRepository* qmlSingleton(QQmlEngine*, QJSEngine*);
    static PersonnelRepository* instance();

    Q_INVOKABLE QVariantList listPersonnel(const QString &nameFilter = QString(), int limit = 500, int offset = 0);
    Q_INVOKABLE QVariantMap personnelDetails(int personesId);

private:
    static PersonnelRepository *s_instance;
};

#endif // PERSONNELREPOSITORY_H
