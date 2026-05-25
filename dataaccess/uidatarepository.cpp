#include "uidatarepository.h"

#include <QDateTime>
#include <QMap>
#include <QStringList>
#include <QVariantMap>

#include "dataaccess.h"
#include "eventsrepository.h"
#include "mapobjectsrepository.h"
#include "personnelrepository.h"

namespace {
QString typeLabel(int objectType)
{
    switch (objectType) {
    case 1: return QStringLiteral("\u0424\u043e\u0440\u043c\u0438\u0440\u043e\u0432\u0430\u043d\u0438\u044f");
    case 2: return QStringLiteral("\u041e\u0441\u043e\u0431\u044b\u0435 \u0443\u0441\u043b\u043e\u0432\u0438\u044f");
    case 3: return QStringLiteral("\u0421\u0440\u0435\u0434\u0441\u0442\u0432\u0430 \u0421\u041c\u0418");
    case 4: return QStringLiteral("\u0421\u0440\u0435\u0434\u0441\u0442\u0432\u0430 \u0444\u043e\u0440\u043c\u0438\u0440\u043e\u0432\u0430\u043d\u0438\u0439");
    case 5: return QStringLiteral("\u0421\u0440\u0435\u0434\u0441\u0442\u0432\u0430 \u0433\u0440\u0443\u043f\u043f");
    case 6: return QStringLiteral("\u0420\u0435\u0433\u0438\u043e\u043d\u044b");
    case 7: return QStringLiteral("\u041f\u0435\u0440\u0441\u043e\u043d\u0430\u043b\u0438\u0438");
    case 8: return QStringLiteral("\u0421\u043e\u0431\u044b\u0442\u0438\u044f");
    case 9: return QStringLiteral("\u0413\u0440\u0443\u043f\u043f\u044b");
    case 10: return QStringLiteral("\u0421\u041c\u0418");
    default: return QStringLiteral("\u041e\u0431\u044a\u0435\u043a\u0442\u044b");
    }
}

QString eventTypeValue(const QString &typeName)
{
    const QString normalized = typeName.trimmed().toLower();
    if (normalized.contains(QStringLiteral("\u043a\u0438\u0431")) || normalized.contains(QStringLiteral("cyber"))) {
        return QStringLiteral("cyber");
    }
    if (normalized.contains(QStringLiteral("\u043e\u0433\u043d\u0435")) || normalized.contains(QStringLiteral("fire"))) {
        return QStringLiteral("fire");
    }
    if (normalized.contains(QStringLiteral("\u0440\u0430\u0437\u0432\u0435\u0434")) || normalized.contains(QStringLiteral("intel"))) {
        return QStringLiteral("intel");
    }
    return QStringLiteral("other");
}

QString eventStatusValue(const QString &statusName, const QVariant &startValue, const QVariant &endValue)
{
    const QString normalized = statusName.trimmed().toLower();
    if (normalized.contains(QStringLiteral("\u043e\u0442\u043c\u0435\u043d")) || normalized.contains(QStringLiteral("cancel"))) {
        return QStringLiteral("cancelled");
    }
    if (normalized.contains(QStringLiteral("\u043f\u043b\u0430\u043d")) || normalized.contains(QStringLiteral("plan"))) {
        return QStringLiteral("planned");
    }
    if (normalized.contains(QStringLiteral("\u0437\u0430\u0432\u0435\u0440")) || normalized.contains(QStringLiteral("past"))) {
        return QStringLiteral("past");
    }
    if (normalized.contains(QStringLiteral("\u0430\u043a\u0442\u0443")) || normalized.contains(QStringLiteral("actual"))) {
        return QStringLiteral("actual");
    }

    const QDateTime nowUtc = QDateTime::currentDateTimeUtc();
    const QDateTime startUtc = startValue.toDateTime().toUTC();
    const QDateTime endUtc = endValue.toDateTime().toUTC();
    if (startUtc.isValid() && nowUtc < startUtc) {
        return QStringLiteral("planned");
    }
    if (endUtc.isValid() && nowUtc > endUtc) {
        return QStringLiteral("past");
    }
    return QStringLiteral("actual");
}

QString eventStatusLabel(const QString &statusKey)
{
    if (statusKey == QStringLiteral("actual")) {
        return QStringLiteral("\u0410\u043a\u0442\u0443\u0430\u043b\u044c\u043d\u044b\u0435");
    }
    if (statusKey == QStringLiteral("planned")) {
        return QStringLiteral("\u041f\u043b\u0430\u043d\u0438\u0440\u0443\u0435\u043c\u044b\u0435");
    }
    if (statusKey == QStringLiteral("past")) {
        return QStringLiteral("\u041f\u0440\u043e\u0448\u0435\u0434\u0448\u0438\u0435");
    }
    if (statusKey == QStringLiteral("cancelled")) {
        return QStringLiteral("\u041e\u0442\u043c\u0435\u043d\u0435\u043d\u043d\u044b\u0435");
    }
    return QStringLiteral("\u0421\u043e\u0431\u044b\u0442\u0438\u044f");
}
} // namespace

UiDataRepository *UiDataRepository::s_instance = nullptr;

UiDataRepository::UiDataRepository(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

UiDataRepository *UiDataRepository::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new UiDataRepository();
    }
    return s_instance;
}

UiDataRepository *UiDataRepository::instance()
{
    if (s_instance == nullptr) {
        s_instance = new UiDataRepository();
    }
    return s_instance;
}

QVariantList UiDataRepository::buildReferenceTree(int objectsLimit, int personnelLimit, int eventsLimit)
{
    QVariantList roots;
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return roots;
    }

    QVariantList objectTypeNodes;
    const QList<int> mapTypes{1, 3, 4, 5, 6, 9, 10};
    for (int mapType : mapTypes) {
        const QVariantList mapRows = MapObjectsRepository::instance()->listObjectsByType(mapType, objectsLimit);
        QVariantList mapLeaves;
        for (const QVariant &rowValue : mapRows) {
            const QVariantMap row = rowValue.toMap();
            QVariantMap leaf;
            leaf.insert(QStringLiteral("id"), QStringLiteral("obj_%1_%2").arg(mapType).arg(row.value(QStringLiteral("id")).toInt()));
            leaf.insert(QStringLiteral("label"),
                        row.value(QStringLiteral("name")).toString().isEmpty()
                            ? QStringLiteral("%1 #%2").arg(typeLabel(mapType)).arg(row.value(QStringLiteral("id")).toInt())
                            : row.value(QStringLiteral("name")).toString());
            leaf.insert(QStringLiteral("subtitle"), row.value(QStringLiteral("subtitle")).toString());
            leaf.insert(QStringLiteral("nodeKind"), QStringLiteral("object"));
            leaf.insert(QStringLiteral("objectType"), mapType);
            leaf.insert(QStringLiteral("objectId"), row.value(QStringLiteral("id")).toInt());
            leaf.insert(QStringLiteral("children"), QVariantList{});
            mapLeaves.append(leaf);
        }

        QVariantMap node;
        node.insert(QStringLiteral("id"), QStringLiteral("map_type_%1").arg(mapType));
        node.insert(QStringLiteral("label"), QStringLiteral("%1 [%2]").arg(typeLabel(mapType)).arg(mapLeaves.size()));
        node.insert(QStringLiteral("children"), mapLeaves);
        objectTypeNodes.append(node);
    }

    const QVariantList personnelRows = PersonnelRepository::instance()->listPersonnel(QString(), personnelLimit, 0);
    QMap<QString, QVariantList> personnelGroups;
    for (const QVariant &rowValue : personnelRows) {
        const QVariantMap row = rowValue.toMap();
        const QString personType = row.value(QStringLiteral("typeName")).toString().isEmpty()
            ? QStringLiteral("\u0411\u0435\u0437 \u0442\u0438\u043f\u0430")
            : row.value(QStringLiteral("typeName")).toString();
        QVariantMap leaf;
        leaf.insert(QStringLiteral("id"), QStringLiteral("pers_%1").arg(row.value(QStringLiteral("id")).toInt()));
        leaf.insert(QStringLiteral("label"),
                    row.value(QStringLiteral("name")).toString().isEmpty()
                        ? QStringLiteral("\u041f\u0435\u0440\u0441\u043e\u043d\u0430\u043b\u0438\u044f #%1").arg(row.value(QStringLiteral("id")).toInt())
                        : row.value(QStringLiteral("name")).toString());
        leaf.insert(QStringLiteral("subtitle"), row.value(QStringLiteral("rank")).toString());
        leaf.insert(QStringLiteral("nodeKind"), QStringLiteral("object"));
        leaf.insert(QStringLiteral("objectType"), 7);
        leaf.insert(QStringLiteral("objectId"), row.value(QStringLiteral("id")).toInt());
        leaf.insert(QStringLiteral("children"), QVariantList{});
        personnelGroups[personType].append(leaf);
    }

    QVariantList personnelNodes;
    int personnelTypeIndex = 0;
    for (auto it = personnelGroups.constBegin(); it != personnelGroups.constEnd(); ++it, ++personnelTypeIndex) {
        QVariantMap node;
        node.insert(QStringLiteral("id"), QStringLiteral("pers_type_%1").arg(personnelTypeIndex));
        node.insert(QStringLiteral("label"), QStringLiteral("%1 [%2]").arg(it.key()).arg(it.value().size()));
        node.insert(QStringLiteral("children"), it.value());
        personnelNodes.append(node);
    }

    const QVariantList specialRows = MapObjectsRepository::instance()->listObjectsByType(2, objectsLimit);
    QMap<QString, QVariantList> specialGroups;
    for (const QVariant &rowValue : specialRows) {
        const QVariantMap row = rowValue.toMap();
        const QString specialType = row.value(QStringLiteral("subtitle")).toString().isEmpty()
            ? QStringLiteral("\u0411\u0435\u0437 \u0442\u0438\u043f\u0430")
            : row.value(QStringLiteral("subtitle")).toString();
        QVariantMap leaf;
        leaf.insert(QStringLiteral("id"), QStringLiteral("sc_%1").arg(row.value(QStringLiteral("id")).toInt()));
        leaf.insert(QStringLiteral("label"),
                    row.value(QStringLiteral("name")).toString().isEmpty()
                        ? QStringLiteral("\u041e\u0441\u043e\u0431\u043e\u0435 \u0443\u0441\u043b\u043e\u0432\u0438\u0435 #%1").arg(row.value(QStringLiteral("id")).toInt())
                        : row.value(QStringLiteral("name")).toString());
        leaf.insert(QStringLiteral("subtitle"), row.value(QStringLiteral("subtitle")).toString());
        leaf.insert(QStringLiteral("nodeKind"), QStringLiteral("object"));
        leaf.insert(QStringLiteral("objectType"), 2);
        leaf.insert(QStringLiteral("objectId"), row.value(QStringLiteral("id")).toInt());
        leaf.insert(QStringLiteral("children"), QVariantList{});
        specialGroups[specialType].append(leaf);
    }

    QVariantList specialNodes;
    int specialTypeIndex = 0;
    for (auto it = specialGroups.constBegin(); it != specialGroups.constEnd(); ++it, ++specialTypeIndex) {
        QVariantMap node;
        node.insert(QStringLiteral("id"), QStringLiteral("sc_type_%1").arg(specialTypeIndex));
        node.insert(QStringLiteral("label"), QStringLiteral("%1 [%2]").arg(it.key()).arg(it.value().size()));
        node.insert(QStringLiteral("children"), it.value());
        specialNodes.append(node);
    }

    const QVariantList eventRows = EventsRepository::instance()->listEvents(QString(), eventsLimit, 0);
    QMap<QString, QMap<QString, QVariantList>> eventStatusGroups;
    for (const QVariant &rowValue : eventRows) {
        const QVariantMap row = rowValue.toMap();
        const QString statusKey = eventStatusValue(
            row.value(QStringLiteral("statusName")).toString(),
            row.value(QStringLiteral("start")),
            row.value(QStringLiteral("end")));
        const QString typeKey = eventTypeValue(row.value(QStringLiteral("typeName")).toString());

        QVariantMap leaf;
        leaf.insert(QStringLiteral("id"), QStringLiteral("event_%1").arg(row.value(QStringLiteral("id")).toInt()));
        leaf.insert(QStringLiteral("label"),
                    row.value(QStringLiteral("name")).toString().isEmpty()
                        ? QStringLiteral("\u0421\u043e\u0431\u044b\u0442\u0438\u0435 #%1").arg(row.value(QStringLiteral("id")).toInt())
                        : row.value(QStringLiteral("name")).toString());
        leaf.insert(QStringLiteral("nodeKind"), QStringLiteral("event"));
        leaf.insert(QStringLiteral("eventId"), row.value(QStringLiteral("id")).toInt());
        leaf.insert(QStringLiteral("eventType"), typeKey);
        leaf.insert(QStringLiteral("eventStatus"), statusKey);
        leaf.insert(QStringLiteral("eventStart"), row.value(QStringLiteral("start")).toDateTime().toString(Qt::ISODate));
        leaf.insert(QStringLiteral("eventEnd"), row.value(QStringLiteral("end")).toDateTime().toString(Qt::ISODate));
        leaf.insert(QStringLiteral("children"), QVariantList{});
        eventStatusGroups[statusKey][typeKey].append(leaf);
    }

    QVariantList eventNodes;
    const QStringList statusOrder{QStringLiteral("actual"), QStringLiteral("planned"), QStringLiteral("past"), QStringLiteral("cancelled")};
    for (const QString &statusName : statusOrder) {
        if (!eventStatusGroups.contains(statusName)) {
            continue;
        }

        QVariantList typeNodes;
        const QMap<QString, QVariantList> typeGroups = eventStatusGroups.value(statusName);
        for (auto it = typeGroups.constBegin(); it != typeGroups.constEnd(); ++it) {
            QVariantMap typeNode;
            typeNode.insert(QStringLiteral("id"), QStringLiteral("event_type_%1_%2").arg(statusName, it.key()));
            typeNode.insert(QStringLiteral("label"), QStringLiteral("%1 [%2]").arg(it.key()).arg(it.value().size()));
            typeNode.insert(QStringLiteral("children"), it.value());
            typeNodes.append(typeNode);
        }

        QVariantMap statusNode;
        statusNode.insert(QStringLiteral("id"), QStringLiteral("event_status_%1").arg(statusName));
        statusNode.insert(QStringLiteral("label"), QStringLiteral("%1 [%2]").arg(eventStatusLabel(statusName)).arg(typeNodes.size()));
        statusNode.insert(QStringLiteral("children"), typeNodes);
        eventNodes.append(statusNode);
    }

    QVariantMap root;
    root.insert(QStringLiteral("id"), QStringLiteral("db_entities"));
    root.insert(QStringLiteral("label"), QStringLiteral("\u0421\u0443\u0449\u043d\u043e\u0441\u0442\u0438 \u0411\u0414"));
    root.insert(QStringLiteral("children"), QVariantList{
        QVariantMap{
            {QStringLiteral("id"), QStringLiteral("db_map_objects")},
            {QStringLiteral("label"), QStringLiteral("\u041e\u0431\u044a\u0435\u043a\u0442\u044b")},
            {QStringLiteral("children"), objectTypeNodes}
        },
        QVariantMap{
            {QStringLiteral("id"), QStringLiteral("db_personnel")},
            {QStringLiteral("label"), QStringLiteral("\u041f\u0435\u0440\u0441\u043e\u043d\u0430\u043b\u0438\u0438")},
            {QStringLiteral("children"), personnelNodes}
        },
        QVariantMap{
            {QStringLiteral("id"), QStringLiteral("db_special_conditions")},
            {QStringLiteral("label"), QStringLiteral("\u041e\u0441\u043e\u0431\u044b\u0435 \u0443\u0441\u043b\u043e\u0432\u0438\u044f")},
            {QStringLiteral("children"), specialNodes}
        },
        QVariantMap{
            {QStringLiteral("id"), QStringLiteral("db_events")},
            {QStringLiteral("label"), QStringLiteral("\u0421\u043e\u0431\u044b\u0442\u0438\u044f")},
            {QStringLiteral("children"), eventNodes}
        }
    });
    roots.append(root);
    return roots;
}

QVariantList UiDataRepository::listMapEvents(int limit)
{
    QVariantList mapped;
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return mapped;
    }

    const QVariantList rows = EventsRepository::instance()->listEvents(QString(), limit, 0);
    for (const QVariant &rowValue : rows) {
        const QVariantMap row = rowValue.toMap();
        const int eventId = row.value(QStringLiteral("id")).toInt();

        QVariantMap item;
        item.insert(QStringLiteral("id"), eventId);
        item.insert(
            QStringLiteral("name"),
            row.value(QStringLiteral("name")).toString().isEmpty()
                ? QStringLiteral("\u0421\u043e\u0431\u044b\u0442\u0438\u0435 %1").arg(eventId)
                : row.value(QStringLiteral("name")).toString());
        item.insert(QStringLiteral("type"), eventTypeValue(row.value(QStringLiteral("typeName")).toString()));
        item.insert(
            QStringLiteral("status"),
            eventStatusValue(
                row.value(QStringLiteral("statusName")).toString(),
                row.value(QStringLiteral("start")),
                row.value(QStringLiteral("end"))));
        item.insert(QStringLiteral("idTypeEvent"), row.value(QStringLiteral("idTypeEvent")).toInt());
        item.insert(QStringLiteral("idEventStatus"), row.value(QStringLiteral("idEventStatus")).toInt());
        item.insert(QStringLiteral("idSign"), row.value(QStringLiteral("idSign")).toInt());
        item.insert(QStringLiteral("signKey"), row.value(QStringLiteral("signKey")).toString());
        item.insert(QStringLiteral("startTimestamp"), row.value(QStringLiteral("start")).toDateTime().toString(Qt::ISODate));
        item.insert(QStringLiteral("endTimestamp"), row.value(QStringLiteral("end")).toDateTime().toString(Qt::ISODate));
        item.insert(QStringLiteral("lon"), row.value(QStringLiteral("lon")).toDouble());
        item.insert(QStringLiteral("lat"), row.value(QStringLiteral("lat")).toDouble());
        item.insert(QStringLiteral("updatedTimestamp"), QString());
        item.insert(QStringLiteral("description"), QString());
        item.insert(QStringLiteral("objectIds"), QVariantList{});
        mapped.append(item);
    }
    return mapped;
}
