#include "localgeojsonrepository.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

LocalGeoJsonRepository *LocalGeoJsonRepository::s_instance = nullptr;

LocalGeoJsonRepository::LocalGeoJsonRepository(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

LocalGeoJsonRepository *LocalGeoJsonRepository::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new LocalGeoJsonRepository();
    }
    return s_instance;
}

LocalGeoJsonRepository *LocalGeoJsonRepository::instance()
{
    if (s_instance == nullptr) {
        s_instance = new LocalGeoJsonRepository();
    }
    return s_instance;
}

QVariantMap LocalGeoJsonRepository::emptyCollection() const
{
    QVariantMap collection;
    collection.insert(QStringLiteral("type"), QStringLiteral("FeatureCollection"));
    collection.insert(QStringLiteral("features"), QVariantList{});
    return collection;
}

QVariantMap LocalGeoJsonRepository::loadGeoJson(const QString &path)
{
    const QString normalizedPath = path.trimmed();
    if (normalizedPath.isEmpty()) {
        return emptyCollection();
    }

    const auto cached = m_cache.constFind(normalizedPath);
    if (cached != m_cache.constEnd()) {
        return cached.value();
    }

    QFile source(normalizedPath);
    if (!source.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return emptyCollection();
    }

    const QByteArray payload = source.readAll();
    source.close();

    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(payload, &error);
    if (error.error != QJsonParseError::NoError || !doc.isObject()) {
        return emptyCollection();
    }

    const QJsonObject root = doc.object();
    if (root.value(QStringLiteral("type")).toString() != QStringLiteral("FeatureCollection")) {
        return emptyCollection();
    }

    const QVariantMap asVariant = root.toVariantMap();
    m_cache.insert(normalizedPath, asVariant);
    return asVariant;
}
