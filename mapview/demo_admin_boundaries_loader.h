#ifndef DEMO_ADMIN_BOUNDARIES_LOADER_H
#define DEMO_ADMIN_BOUNDARIES_LOADER_H

#include <QObject>
#include <QVariantList>
#include <QJsonArray>
#include <QJsonObject>
#include <qqmlintegration.h>

class DemoAdminBoundariesLoader : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit DemoAdminBoundariesLoader(QObject *parent = nullptr);

    Q_INVOKABLE QVariantList loadDefaultPolygons() const;
    Q_INVOKABLE QVariantList loadPolygonsFromResource(const QString &resourcePath) const;

private:
    QVariantList parseFeatureCollection(const QByteArray &jsonPayload) const;
    void appendFeaturePolygons(const QJsonObject &feature, QVariantList &outPolygons) const;
    QVariantMap buildPolygon(const QString &featureName, const QJsonArray &ringCoordinates) const;
};

#endif // DEMO_ADMIN_BOUNDARIES_LOADER_H
