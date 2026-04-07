#include "dbconnectionsettings.h"

#include <QSettings>

#include "dataaccess.h"

DbConnectionSettings *DbConnectionSettings::s_instance = nullptr;

DbConnectionSettings::DbConnectionSettings(QObject *parent)
    : QObject(parent)
    , m_settings("vka_db", "saturn")
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
    load();
}

DbConnectionSettings *DbConnectionSettings::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new DbConnectionSettings();
    }
    return s_instance;
}

DbConnectionSettings *DbConnectionSettings::instance()
{
    if (s_instance == nullptr) {
        s_instance = new DbConnectionSettings();
    }
    return s_instance;
}

QString DbConnectionSettings::driver() const
{
    return m_driver;
}

QString DbConnectionSettings::host() const
{
    return m_host;
}

int DbConnectionSettings::port() const
{
    return m_port;
}

QString DbConnectionSettings::dbName() const
{
    return m_dbName;
}

QString DbConnectionSettings::user() const
{
    return m_user;
}

QString DbConnectionSettings::password() const
{
    return m_password;
}

void DbConnectionSettings::setDriver(const QString &value)
{
    if (m_driver == value) {
        return;
    }
    m_driver = value;
    emit changed();
}

void DbConnectionSettings::setHost(const QString &value)
{
    if (m_host == value) {
        return;
    }
    m_host = value;
    emit changed();
}

void DbConnectionSettings::setPort(int value)
{
    if (m_port == value) {
        return;
    }
    m_port = value;
    emit changed();
}

void DbConnectionSettings::setDbName(const QString &value)
{
    if (m_dbName == value) {
        return;
    }
    m_dbName = value;
    emit changed();
}

void DbConnectionSettings::setUser(const QString &value)
{
    if (m_user == value) {
        return;
    }
    m_user = value;
    emit changed();
}

void DbConnectionSettings::setPassword(const QString &value)
{
    if (m_password == value) {
        return;
    }
    m_password = value;
    emit changed();
}

void DbConnectionSettings::load()
{
    m_driver = m_settings.value("/settings_db_connect/db_driver", "QPSQL").toString();
    m_host = m_settings.value("/settings_db_connect/host", "localhost").toString();
    m_port = m_settings.value("/settings_db_connect/port", 5432).toInt();
    m_dbName = m_settings.value("/settings_db_connect/db_name", "SATURN").toString();
    m_user = m_settings.value("/settings_db_connect/user_name", "postgres").toString();
    m_password = m_settings.value("/settings_db_connect/user_password", "").toString();

    // Legacy compatibility: read old app-scoped QSettings if dedicated storage is empty.
    if (m_password.isEmpty()) {
        QSettings legacy("vka", "saturn");
        if (m_driver == "QPSQL") {
            const QString legacyPassword = legacy.value("/settings_db_connect/user_password", "").toString();
            if (!legacyPassword.isEmpty()) {
                m_password = legacyPassword;
            }
        }
        if (m_host == "localhost") {
            const QString legacyHost = legacy.value("/settings_db_connect/host", "").toString();
            if (!legacyHost.isEmpty()) {
                m_host = legacyHost;
            }
        }
        if (m_dbName.compare("SATURN", Qt::CaseInsensitive) == 0) {
            const QString legacyDbName = legacy.value("/settings_db_connect/db_name", "").toString();
            if (!legacyDbName.isEmpty()) {
                m_dbName = legacyDbName;
            }
        }
        if (m_user.compare("postgres", Qt::CaseInsensitive) == 0) {
            const QString legacyUser = legacy.value("/settings_db_connect/user_name", "").toString();
            if (!legacyUser.isEmpty()) {
                m_user = legacyUser;
            }
        }
    }

    // Optional operational fallback: allow password from environment when not persisted.
    if (m_password.isEmpty()) {
        const QString envPassword = qEnvironmentVariable("SATURN_DB_PASSWORD");
        if (!envPassword.isEmpty()) {
            m_password = envPassword;
        } else {
            const QString pgPassword = qEnvironmentVariable("PGPASSWORD");
            if (!pgPassword.isEmpty()) {
                m_password = pgPassword;
            }
        }
    }
    emit changed();
}

void DbConnectionSettings::save()
{
    m_settings.setValue("/settings_db_connect/db_driver", m_driver);
    m_settings.setValue("/settings_db_connect/host", m_host);
    m_settings.setValue("/settings_db_connect/port", m_port);
    m_settings.setValue("/settings_db_connect/db_name", m_dbName);
    m_settings.setValue("/settings_db_connect/user_name", m_user);
    m_settings.setValue("/settings_db_connect/user_password", m_password);
    m_settings.sync();
}

bool DbConnectionSettings::applyAndConnect()
{
    DataAccess *db = DataAccess::instance();
    db->setDriver(m_driver);
    db->setHost(m_host);
    db->setPort(m_port);
    db->setDbName(m_dbName);
    db->setUser(m_user);
    db->setPassword(m_password);
    return db->connectToDatabase();
}
