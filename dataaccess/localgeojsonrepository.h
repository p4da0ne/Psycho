#ifndef LOCALGEOJSONREPOSITORY_H
#define LOCALGEOJSONREPOSITORY_H

#include <QHash>
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QVariantMap>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS LocalGeoJsonRepository : public QObject
{
    Q_OBJECT

public:
    explicit LocalGeoJsonRepository(QObject *parent = nullptr);

    static LocalGeoJsonRepository* qmlSingleton(QQmlEngine*, QJSEngine*);
    static LocalGeoJsonRepository* instance();

    Q_INVOKABLE QVariantMap loadGeoJson(const QString &path);

private:
    QVariantMap emptyCollection() const;

    static LocalGeoJsonRepository *s_instance;
    QHash<QString, QVariantMap> m_cache;
};

#endif // LOCALGEOJSONREPOSITORY_H
