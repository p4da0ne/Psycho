#include "pollingservice.h"

PollingService *PollingService::s_instance = nullptr;

PollingService::PollingService(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }

    m_timer.setInterval(m_intervalMs);
    connect(&m_timer, &QTimer::timeout, this, &PollingService::tick);
}

PollingService *PollingService::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new PollingService();
    }
    return s_instance;
}

PollingService *PollingService::instance()
{
    if (s_instance == nullptr) {
        s_instance = new PollingService();
    }
    return s_instance;
}

bool PollingService::running() const
{
    return m_timer.isActive();
}

int PollingService::intervalMs() const
{
    return m_intervalMs;
}

void PollingService::setIntervalMs(int intervalMs)
{
    if (intervalMs < 1000) {
        intervalMs = 1000;
    }
    if (m_intervalMs == intervalMs) {
        return;
    }
    m_intervalMs = intervalMs;
    m_timer.setInterval(m_intervalMs);
    emit intervalChanged();
}

void PollingService::start()
{
    if (m_timer.isActive()) {
        return;
    }
    m_timer.start();
    emit runningChanged();
}

void PollingService::stop()
{
    if (!m_timer.isActive()) {
        return;
    }
    m_timer.stop();
    emit runningChanged();
}

void PollingService::triggerNow()
{
    emit tick();
}
