#include "dataaccess.h"

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QDebug>

namespace {
QString normalizeDriverName(const QString &driverName)
{
    QString normalized = driverName.trimmed().toUpper();
    if (normalized == "QPSQL7") {
        normalized = "QPSQL";
    }
    return normalized;
}
}

DataAccess *DataAccess::s_instance = nullptr;

DataAccess::DataAccess(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

DataAccess::~DataAccess()
{
    if (m_db.isValid() && m_db.isOpen()) {
        m_db.close();
    }
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

DataAccess *DataAccess::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new DataAccess();
    }
    return s_instance;
}

DataAccess *DataAccess::instance()
{
    if (s_instance == nullptr) {
        s_instance = new DataAccess();
    }
    return s_instance;
}

bool DataAccess::connected() const
{
    return m_db.isValid() && m_db.isOpen();
}

QString DataAccess::lastError() const
{
    return m_lastError;
}

QString DataAccess::driver() const
{
    return m_driver;
}

bool DataAccess::connectToDatabase()
{
    if (connected()) {
        return true;
    }

    const QString normalizedDriver = normalizeDriverName(m_driver);
    if (normalizedDriver.isEmpty()) {
        m_lastError = "Не указан драйвер БД";
        emit errorOccurred();
        emit connectionChanged();
        return false;
    }
    if (normalizedDriver != m_driver) {
        m_driver = normalizedDriver;
        emit settingsChanged();
    }
    if (m_host.trimmed().isEmpty()) {
        m_lastError = "Не указан хост БД";
        emit errorOccurred();
        emit connectionChanged();
        return false;
    }
    if (m_dbName.trimmed().isEmpty()) {
        m_lastError = "Не указано имя БД";
        emit errorOccurred();
        emit connectionChanged();
        return false;
    }
    if (m_user.trimmed().isEmpty()) {
        m_lastError = "Не указан пользователь БД";
        emit errorOccurred();
        emit connectionChanged();
        return false;
    }
    if (m_driver == "QPSQL" && m_password.isEmpty()) {
        m_lastError = "Не задан пароль подключения к PostgreSQL. Откройте 'Настройки подключения к БД' и заполните пароль.";
        emit errorOccurred();
        emit connectionChanged();
        return false;
    }

    return create_connection(m_driver, m_dbName, m_user, m_password, m_host, m_port);
}

bool DataAccess::connect()
{
    return connectToDatabase();
}

bool DataAccess::disconnectDatabase()
{
    return close_connection_();
}

void DataAccess::setDriver(const QString &v)
{
    const QString normalized = normalizeDriverName(v);
    if (normalized.isEmpty() || m_driver == normalized) {
        return;
    }
    m_driver = normalized;
    emit settingsChanged();
}

void DataAccess::setHost(const QString &v)
{
    if (m_host == v) {
        return;
    }
    m_host = v;
    emit settingsChanged();
}

void DataAccess::setPort(int v)
{
    if (m_port == v) {
        return;
    }
    m_port = v;
    emit settingsChanged();
}

void DataAccess::setDbName(const QString &v)
{
    if (m_dbName == v) {
        return;
    }
    m_dbName = v;
    emit settingsChanged();
}

void DataAccess::setUser(const QString &v)
{
    if (m_user == v) {
        return;
    }
    m_user = v;
    emit settingsChanged();
}

void DataAccess::setPassword(const QString &v)
{
    if (m_password == v) {
        return;
    }
    m_password = v;
    emit settingsChanged();
}

bool DataAccess::create_connection(QString db_driver, QString database, QString user, QString user_password, QString hostname, int port)
{
    m_driver = normalizeDriverName(db_driver);
    m_dbName = database;
    m_user = user;
    m_password = user_password;
    m_host = hostname;
    m_port = port;

    m_db = ensureDatabase();
    if (!m_db.isValid()) {
        const QStringList drivers = QSqlDatabase::drivers();
        m_lastError = QString("Драйвер БД '%1' не загружен. Доступные драйверы: %2")
                          .arg(m_driver, drivers.join(", "));
        qWarning().noquote()
            << "DB driver load failed;"
            << "requested=" << m_driver
            << "; availableDrivers=" << drivers.join(",")
            << "; libraryPaths=" << QCoreApplication::libraryPaths().join(";");
        m_connectionFlag = false;
        emit errorOccurred();
        emit connectionChanged();
        return false;
    }
    m_db.setDatabaseName(m_dbName);
    m_db.setUserName(m_user);
    m_db.setPassword(m_password);
    m_db.setHostName(m_host);
    m_db.setPort(m_port);
    if (m_driver == "QPSQL") {
        // Prevent long UI stalls on unreachable hosts during sync connect().
        m_db.setConnectOptions("connect_timeout=5");
    } else {
        m_db.setConnectOptions(QString());
    }

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qWarning().noquote()
            << "DB open failed;"
            << "driver=" << m_driver
            << "; error=" << m_lastError
            << "; db=" << m_dbName
            << "; host=" << m_host
            << "; port=" << m_port
            << "; user=" << m_user
            << "; availableDrivers=" << QSqlDatabase::drivers().join(",")
            << "; libraryPaths=" << QCoreApplication::libraryPaths().join(";");
        m_connectionFlag = false;
        emit errorOccurred();
        emit connectionChanged();
        return false;
    }

    m_lastError = "Соединение с базой данных установлено";
    m_connectionFlag = true;
    emit connectionChanged();
    return true;
}

bool DataAccess::get_connection_flag()
{
    return m_connectionFlag;
}

QString DataAccess::get_message()
{
    return m_lastError;
}

bool DataAccess::close_connection_()
{
    if (m_db.isValid() && m_db.isOpen()) {
        m_db.close();
    }
    const bool isClosed = !m_db.isOpen();
    m_connectionFlag = !isClosed ? m_connectionFlag : false;
    emit connectionChanged();
    return isClosed;
}

QString DataAccess::get_user()
{
    return m_user;
}

QString DataAccess::get_user_password()
{
    return m_password;
}

QString DataAccess::get_hostname()
{
    return m_host;
}

void DataAccess::set_user(QString new_user)
{
    setUser(new_user);
}

void DataAccess::set_user_password(QString new_user_password)
{
    setPassword(new_user_password);
}

void DataAccess::set_hostname(QString new_hostname)
{
    setHost(new_hostname);
}

QString DataAccess::defaultConnectionName()
{
    // Keep Qt default SQL connection for compatibility with existing code paths
    // that use QSqlQuery() without explicitly passing a connection.
    return QSqlDatabase::defaultConnection;
}

QSqlDatabase DataAccess::ensureDatabase()
{
    const QString connectionName = defaultConnectionName();
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        if (db.driverName() != m_driver) {
            if (db.isOpen()) {
                db.close();
            }
            db = QSqlDatabase();
            QSqlDatabase::removeDatabase(connectionName);
        } else {
            return db;
        }
    }
    return QSqlDatabase::addDatabase(m_driver, connectionName);
}
