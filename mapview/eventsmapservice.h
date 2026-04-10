#ifndef EVENTSMAPSERVICE_H
#define EVENTSMAPSERVICE_H

#include <QObject>

class EventsMapService : public QObject
{
    Q_OBJECT
public:
    explicit EventsMapService(QObject *parent = nullptr);

signals:
};

struct EventFilter {
    QDateTime from;
    QDateTime to;

    QList<int> typeIds;
    QList<int> statusIds;

    bool onlyActive = false;
    bool onlyWithCoordinates = true;

    QRectF bbox;              // или свой тип для гео-прямоугольника
    bool useBbox = false;

    QString text;
    int limit = 5000;
};

struct MapMarkerData {
    qint64 id;
    QGeoCoordinate coordinate;

    QString category;     // "event", "camera", "alert"
    QString iconKey;      // "alarm-red", "camera-blue"
    QString title;

    bool selected = false;
    bool visible = true;

    QVariantMap properties;
};

#endif // EVENTSMAPSERVICE_H

