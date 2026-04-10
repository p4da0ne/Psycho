#include "geojsontilefeedservice.h"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

#include "mapruntimeservice.h"

namespace {
QString featureCollectionTemplate()
{
    return QStringLiteral(R"({"type":"FeatureCollection","features":[]})");
}
}

GeoJsonTileFeedService *GeoJsonTileFeedService::s_instance = nullptr;

GeoJsonTileFeedService::GeoJsonTileFeedService(QObject *parent)
    : QObject(parent)
    , m_outputDir(defaultOutputDir())
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
    bindRuntime();
}

GeoJsonTileFeedService *GeoJsonTileFeedService::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    return instance();
}

GeoJsonTileFeedService *GeoJsonTileFeedService::instance()
{
    if (s_instance == nullptr) {
        s_instance = new GeoJsonTileFeedService();
    }
    s_instance->bindRuntime();
    return s_instance;
}

bool GeoJsonTileFeedService::enabled() const
{
    return m_enabled;
}

void GeoJsonTileFeedService::setEnabled(bool enabled)
{
    if (m_enabled == enabled) {
        return;
    }
    m_enabled = enabled;
    emit enabledChanged();
    emit stateChanged();
}

QString GeoJsonTileFeedService::outputDir() const
{
    return m_outputDir;
}

void GeoJsonTileFeedService::setOutputDir(const QString &outputDir)
{
    const QString normalized = normalizedDirPath(outputDir);
    if (normalized.isEmpty() || normalized == m_outputDir) {
        return;
    }
    m_outputDir = normalized;
    emit outputDirChanged();
    emit stateChanged();
}

QString GeoJsonTileFeedService::lastError() const
{
    return m_lastError;
}

QString GeoJsonTileFeedService::lastPublishIso() const
{
    return m_lastPublishIso;
}

int GeoJsonTileFeedService::publishCount() const
{
    return m_publishCount;
}

bool GeoJsonTileFeedService::publishNow()
{
    MapRuntimeService *runtime = MapRuntimeService::instance();
    return publishCollections(
        runtime->pointsSource(),
        runtime->linesSource(),
        runtime->polygonsSource());
}

void GeoJsonTileFeedService::bindRuntime()
{
    if (m_runtimeBound) {
        return;
    }
    MapRuntimeService *runtime = MapRuntimeService::instance();
    connect(
        runtime,
        &MapRuntimeService::sourcesUpdated,
        this,
        [this, runtime]() {
            if (!m_enabled) {
                return;
            }
            publishCollections(runtime->pointsSource(), runtime->linesSource(), runtime->polygonsSource());
        });
    m_runtimeBound = true;
}

bool GeoJsonTileFeedService::publishCollections(
    const QString &pointsSource,
    const QString &linesSource,
    const QString &polygonsSource)
{
    QString error;
    const QString dirPath = normalizedDirPath(m_outputDir);
    if (dirPath.isEmpty()) {
        m_lastError = QStringLiteral("Tile feed output directory is empty");
        emit stateChanged();
        return false;
    }

    QDir outputDir(dirPath);
    if (!outputDir.exists() && !outputDir.mkpath(QStringLiteral("."))) {
        m_lastError = QStringLiteral("Failed to create output directory: %1").arg(dirPath);
        emit stateChanged();
        return false;
    }

    const QString pointsPayload = pointsSource.trimmed().isEmpty() ? featureCollectionTemplate() : pointsSource;
    const QString linesPayload = linesSource.trimmed().isEmpty() ? featureCollectionTemplate() : linesSource;
    const QString polygonsPayload = polygonsSource.trimmed().isEmpty() ? featureCollectionTemplate() : polygonsSource;
    const QString mergedPayload = mergedCollection(pointsPayload, linesPayload, polygonsPayload);

    if (!writeAtomicTextFile(outputDir.filePath(QStringLiteral("points.geojson")), pointsPayload, error)
        || !writeAtomicTextFile(outputDir.filePath(QStringLiteral("lines.geojson")), linesPayload, error)
        || !writeAtomicTextFile(outputDir.filePath(QStringLiteral("polygons.geojson")), polygonsPayload, error)
        || !writeAtomicTextFile(outputDir.filePath(QStringLiteral("markers.geojson")), mergedPayload, error)) {
        m_lastError = error;
        emit stateChanged();
        return false;
    }

    const QString publishedAt = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    QJsonObject manifest;
    manifest.insert(QStringLiteral("publishedAt"), publishedAt);
    manifest.insert(QStringLiteral("points"), QStringLiteral("points.geojson"));
    manifest.insert(QStringLiteral("lines"), QStringLiteral("lines.geojson"));
    manifest.insert(QStringLiteral("polygons"), QStringLiteral("polygons.geojson"));
    manifest.insert(QStringLiteral("markers"), QStringLiteral("markers.geojson"));
    manifest.insert(QStringLiteral("publishCount"), m_publishCount + 1);
    const QString manifestPayload =
        QString::fromUtf8(QJsonDocument(manifest).toJson(QJsonDocument::Compact));
    if (!writeAtomicTextFile(outputDir.filePath(QStringLiteral("manifest.json")), manifestPayload, error)) {
        m_lastError = error;
        emit stateChanged();
        return false;
    }

    m_lastError.clear();
    m_lastPublishIso = publishedAt;
    ++m_publishCount;
    emit stateChanged();
    return true;
}

bool GeoJsonTileFeedService::writeAtomicTextFile(
    const QString &path,
    const QString &payload,
    QString &error)
{
    QFileInfo fileInfo(path);
    QDir parentDir = fileInfo.dir();
    if (!parentDir.exists() && !parentDir.mkpath(QStringLiteral("."))) {
        error = QStringLiteral("Failed to create parent directory for: %1").arg(path);
        return false;
    }

    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        error = QStringLiteral("Failed to open file for writing: %1").arg(path);
        return false;
    }
    const QByteArray data = payload.toUtf8();
    if (file.write(data) != data.size()) {
        error = QStringLiteral("Failed to write file: %1").arg(path);
        file.cancelWriting();
        return false;
    }
    if (!file.commit()) {
        error = QStringLiteral("Failed to commit file: %1").arg(path);
        return false;
    }
    return true;
}

QString GeoJsonTileFeedService::normalizedDirPath(const QString &path)
{
    QString normalized = path.trimmed();
    if (normalized.isEmpty()) {
        return QString();
    }
    normalized = QDir::fromNativeSeparators(normalized);
    while (normalized.endsWith('/')) {
        normalized.chop(1);
    }
    return normalized;
}

QString GeoJsonTileFeedService::defaultOutputDir()
{
    const QByteArray envValue = qgetenv("SATURN_TILE_FEED_DIR");
    if (!envValue.isEmpty()) {
        return normalizedDirPath(QString::fromLocal8Bit(envValue));
    }

    // Default to repo-local tileserver-gl folder near the build output.
    const QDir appDir(QCoreApplication::applicationDirPath());
    const QString candidate = appDir.absoluteFilePath(QStringLiteral("../../tileserver-gl/styles/maptiler-basic/dynamic"));
    return normalizedDirPath(QDir(candidate).absolutePath());
}

QString GeoJsonTileFeedService::mergedCollection(
    const QString &pointsSource,
    const QString &linesSource,
    const QString &polygonsSource)
{
    QJsonArray mergedFeatures;
    const QStringList sources = { pointsSource, linesSource, polygonsSource };
    for (const QString &source : sources) {
        const QJsonDocument doc = QJsonDocument::fromJson(source.toUtf8());
        if (!doc.isObject()) {
            continue;
        }
        const QJsonArray features = doc.object().value(QStringLiteral("features")).toArray();
        for (const QJsonValue &featureValue : features) {
            mergedFeatures.append(featureValue);
        }
    }

    QJsonObject collection;
    collection.insert(QStringLiteral("type"), QStringLiteral("FeatureCollection"));
    collection.insert(QStringLiteral("features"), mergedFeatures);
    return QString::fromUtf8(QJsonDocument(collection).toJson(QJsonDocument::Compact));
}
