#ifndef LEGACYCALCULATIONSERVICE_H
#define LEGACYCALCULATIONSERVICE_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QSqlDatabase>
#include <QSet>
#include <QString>
#include <QVariantMap>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS LegacyCalculationService : public QObject
{
    Q_OBJECT

public:
    explicit LegacyCalculationService(QObject *parent = nullptr);

    static LegacyCalculationService* qmlSingleton(QQmlEngine*, QJSEngine*);
    static LegacyCalculationService* instance();

    // Returns a map with normalized score/risk and legacy details for heatmap.
    Q_INVOKABLE QVariantMap objectMetrics(int objectType, int objectId);
    QVariantMap objectMetricsWithDb(int objectType, int objectId, const QSqlDatabase &db);

private:
    static LegacyCalculationService *s_instance;

    QVariantMap objectMetricsInternal(int objectType, int objectId, const QSqlDatabase &db, QSet<QString> &guard);
    QVariantMap formationMetrics(int lsId, const QSqlDatabase &db);
    QVariantMap regionMetrics(int regionId, const QSqlDatabase &db);
    QVariantMap mpoMetrics(int mpoId, const QSqlDatabase &db, QSet<QString> &guard);
    QVariantMap groupMetrics(int groupId, const QSqlDatabase &db);
    QVariantMap personnelMetrics(int personesId, const QSqlDatabase &db, QSet<QString> &guard);
    QVariantMap specialConditionMetrics(int specialConditionId, const QSqlDatabase &db);
    QVariantMap eventMetrics(int eventId, const QSqlDatabase &db, QSet<QString> &guard);

    static QVariantMap neutralMetrics(const QString &source, const QString &reason = QString());
    static int tableNameToObjectType(const QString &tableName, int mpoLsId, int mpoSmiId, int mpoGroupsId);
    static double clamp01(double value);
    static int scoreToHeatClass(double heatValue);
    static int mpsLegacyClass(double mpsValue);
    static int regionLegacyClass(double regionValue);
};

#endif // LEGACYCALCULATIONSERVICE_H
