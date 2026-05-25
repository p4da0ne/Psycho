#ifndef MAPSNAPSHOTSERVICE_H
#define MAPSNAPSHOTSERVICE_H

#include <QDateTime>
#include <QFutureWatcher>
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QMap>
#include <QSet>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS MapSnapshotService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastUpdatedIso READ lastUpdatedIso NOTIFY snapshotUpdated)
    Q_PROPERTY(bool pollingBound READ pollingBound NOTIFY pollingBindingChanged)

public:
    explicit MapSnapshotService(QObject *parent = nullptr);

    static MapSnapshotService* qmlSingleton(QQmlEngine*, QJSEngine*);
    static MapSnapshotService* instance();

    QString lastUpdatedIso() const;
    bool pollingBound() const;

    // Force refresh and return combined FeatureCollection for all map types.
    Q_INVOKABLE QString refreshAll();
    Q_INVOKABLE QString refreshAllWithLimit(int limit);
    // Delta refresh by object_geometry(updated_at/count) and return changed object types.
    Q_INVOKABLE QVariantList refreshByDelta(int limit = 5000);

    // Refresh and return FeatureCollection for specific object type.
    Q_INVOKABLE QString refreshType(int objectType, int limit = 5000);

    // Get cached collection (if missing, build once).
    Q_INVOKABLE QString featureCollectionForType(int objectType);
    Q_INVOKABLE QString featureCollectionAll();

    // Returns metadata list: [{objectType, featureCount, refreshedAtIso}]
    Q_INVOKABLE QVariantList snapshotMeta() const;

signals:
    void snapshotUpdated();
    void typeUpdated(int objectType);
    void pollingBindingChanged();

private:
    static MapSnapshotService *s_instance;

    void bindPollingIfNeeded();
    void requestWorkerBuild(const QList<int> &objectTypes, int limit, bool fullRefresh);
    void startWorkerBuild(const QList<int> &objectTypes, int limit, bool fullRefresh);
    void handleWorkerFinished();
    bool readGeometryMeta(int objectType, QDateTime &maxUpdatedUtc, int &rowCount) const;
    bool readChangedTypesFromChangeLog(QSet<int> &changedTypes, qint64 &maxChangeId) const;
    bool readLastChangeId(qint64 &lastChangeId) const;
    static int countFeaturesInCollection(const QString &geoJson);
    static QList<int> supportedMapTypes();

    bool m_pollingBound = false;
    QDateTime m_lastUpdated;
    QMap<int, QString> m_geoJsonByType;
    QMap<int, int> m_featureCountByType;
    QMap<int, QDateTime> m_refreshedByType;
    QMap<int, QDateTime> m_lastGeometryUpdatedByType;
    QMap<int, int> m_lastGeometryRowsByType;
    bool m_deltaBaselineInitialized = false;
    qint64 m_lastProcessedChangeId = 0;

    QFutureWatcher<QVariantMap> m_workerWatcher;
    bool m_workerBusy = false;
    bool m_pendingFullRefresh = false;
    QSet<int> m_pendingTypes;
    int m_pendingLimit = 5000;
};

#endif // MAPSNAPSHOTSERVICE_H
