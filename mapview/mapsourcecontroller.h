#ifndef MAPSOURCECONTROLLER_H
#define MAPSOURCECONTROLLER_H

#include <QObject>

class MapSourceController : public QObject
{
    Q_OBJECT
public:
    explicit MapSourceController(QObject *parent = nullptr);

    void setMapObject(QObject* mapObject); // или конкретный тип MapLibre map
    void initialize();

    void updateEventsSource(const QByteArray& geoJson);
    void ensureStyleLayers();

    void setSelectedMarker(qint64 id);
    void clearSelection();

signals:
    void sourceUpdated();
};

#endif // MAPSOURCECONTROLLER_H
