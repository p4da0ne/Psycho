#include "dataaccess.h"

#include <QSqlError>

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
    if (m_driver == v || v.isEmpty()) {
        return;
    }
    m_driver = v;
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
    m_driver = db_driver;
    m_dbName = database;
    m_user = user;
    m_password = user_password;
    m_host = hostname;
    m_port = port;

    m_db = ensureDatabase();
    m_db.setDatabaseName(m_dbName);
    m_db.setUserName(m_user);
    m_db.setPassword(m_password);
    m_db.setHostName(m_host);
    m_db.setPort(m_port);

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
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
    return "saturn_main_connection";
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
