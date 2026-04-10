#ifndef GEOJSONTILEFEEDSERVICE_H
#define GEOJSONTILEFEEDSERVICE_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS GeoJsonTileFeedService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QString outputDir READ outputDir WRITE setOutputDir NOTIFY outputDirChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY stateChanged)
    Q_PROPERTY(QString lastPublishIso READ lastPublishIso NOTIFY stateChanged)
    Q_PROPERTY(int publishCount READ publishCount NOTIFY stateChanged)

public:
    explicit GeoJsonTileFeedService(QObject *parent = nullptr);

    static GeoJsonTileFeedService* qmlSingleton(QQmlEngine*, QJSEngine*);
    static GeoJsonTileFeedService* instance();

    bool enabled() const;
    void setEnabled(bool enabled);

    QString outputDir() const;
    void setOutputDir(const QString &outputDir);

    QString lastError() const;
    QString lastPublishIso() const;
    int publishCount() const;

    Q_INVOKABLE bool publishNow();

signals:
    void enabledChanged();
    void outputDirChanged();
    void stateChanged();

private:
    static GeoJsonTileFeedService *s_instance;

    void bindRuntime();
    bool publishCollections(
        const QString &pointsSource,
        const QString &linesSource,
        const QString &polygonsSource);
    static bool writeAtomicTextFile(const QString &path, const QString &payload, QString &error);
    static QString normalizedDirPath(const QString &path);
    static QString defaultOutputDir();
    static QString mergedCollection(
        const QString &pointsSource,
        const QString &linesSource,
        const QString &polygonsSource);

    bool m_runtimeBound = false;
    bool m_enabled = true;
    QString m_outputDir;
    QString m_lastError;
    QString m_lastPublishIso;
    int m_publishCount = 0;
};

#endif // GEOJSONTILEFEEDSERVICE_H
