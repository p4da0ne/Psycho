#ifndef EVENTSREPOSITORY_H
#define EVENTSREPOSITORY_H

#include <QObject>

class EventsRepository : public QObject
{
    Q_OBJECT
public:
    explicit EventsRepository(QObject *parent = nullptr);

signals:
};

struct EventRecord {
    qint64 id;
    QString title;
    QDateTime occurredAt;
    double lon;
    double lat;
    int typeId;
    int statusId;
    QString address;
    QString description;
};

#endif // EVENTSREPOSITORY_H
