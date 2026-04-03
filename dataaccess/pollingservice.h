#ifndef POLLINGSERVICE_H
#define POLLINGSERVICE_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QTimer>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS PollingService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(int intervalMs READ intervalMs WRITE setIntervalMs NOTIFY intervalChanged)

public:
    explicit PollingService(QObject *parent = nullptr);

    static PollingService* qmlSingleton(QQmlEngine*, QJSEngine*);
    static PollingService* instance();

    bool running() const;
    int intervalMs() const;

    void setIntervalMs(int intervalMs);

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void triggerNow();

signals:
    void tick();
    void runningChanged();
    void intervalChanged();

private:
    static PollingService *s_instance;

    QTimer m_timer;
    int m_intervalMs = 30000;
};

#endif // POLLINGSERVICE_H
