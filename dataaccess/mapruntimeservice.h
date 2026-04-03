#ifndef MAPRUNTIMESERVICE_H
#define MAPRUNTIMESERVICE_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QVariantList>
#include <QMap>
#include <QJsonArray>
#include <QTimer>
#include <QSet>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS MapRuntimeService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString pointsSource READ pointsSource NOTIFY sourcesUpdated)
    Q_PROPERTY(QString linesSource READ linesSource NOTIFY sourcesUpdated)
    Q_PROPERTY(QString polygonsSource READ polygonsSource NOTIFY sourcesUpdated)
    Q_PROPERTY(QString lastRuntimeUpdateIso READ lastRuntimeUpdateIso NOTIFY sourcesUpdated)

public:
    explicit MapRuntimeService(QObject *parent = nullptr);

    static MapRuntimeService* qmlSingleton(QQmlEngine*, QJSEngine*);
    static MapRuntimeService* instance();

    QString pointsSource() const;
    QString linesSource() const;
    QString polygonsSource() const;
    QString lastRuntimeUpdateIso() const;

    Q_INVOKABLE void refreshNow();
    Q_INVOKABLE QString sourceForGeometry(const QString &geometryFamily) const;
    Q_INVOKABLE QVariantList updateMeta() const;

signals:
    void sourcesUpdated();

private:
    static MapRuntimeService *s_instance;

    void bindSnapshot();
    void bindEditing();
    void rebuildSources(const QString &fullCollectionJson);
    void scheduleMergedRebuild();
    void scheduleTypeRefresh(int objectType);
    void flushPendingTypeRefreshes();
    void updateTypeCaches(int objectType, const QString &typeCollectionJson);
    void rebuildMergedSourcesFromTypeCaches();
    void rebuildAllFromSnapshotCache();
    static void splitCollectionByGeometry(
        const QString &collectionJson,
        QJsonArray &points,
        QJsonArray &lines,
        QJsonArray &polygons);
    static QList<int> extractTypesFromSnapshotMeta(const QVariantList &meta);

    bool m_snapshotBound = false;
    bool m_editingBound = false;
    QString m_pointsSource;
    QString m_linesSource;
    QString m_polygonsSource;
    QString m_lastRuntimeUpdateIso;
    int m_pointsCount = 0;
    int m_linesCount = 0;
    int m_polygonsCount = 0;
    QTimer m_rebuildDebounceTimer;
    QTimer m_refreshDebounceTimer;
    QSet<int> m_pendingRefreshTypes;
    int m_rebuildDebounceMs = 75;
    int m_refreshDebounceMs = 40;
    QMap<int, QJsonArray> m_pointsByType;
    QMap<int, QJsonArray> m_linesByType;
    QMap<int, QJsonArray> m_polygonsByType;
};

#endif // MAPRUNTIMESERVICE_H
