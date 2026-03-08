#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QQmlEngine>
#include "config_dataaccess.h"

class __EXPORT_DATAACCESS DataAccess : public QObject

{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectionChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY errorOccurred)

    Q_PROPERTY(QString host WRITE setHost)
    Q_PROPERTY(int    port WRITE setPort)
    Q_PROPERTY(QString dbName WRITE setDbName)
    Q_PROPERTY(QString user WRITE setUser)
    Q_PROPERTY(QString password WRITE setPassword)
public:
    explicit DataAccess(QObject *parent = nullptr);

    static DataAccess* qmlSingleton(QQmlEngine *, QJSEngine *);

    bool connected() const { return m_db.isOpen(); }
    QString lastError() const { return m_lastError; }

    Q_INVOKABLE bool connect();

    void setHost(const QString &v)     { m_host = v; }
    void setPort(int v)                { m_port = v; }
    void setDbName(const QString &v)   { m_dbName = v; }
    void setUser(const QString &v)     { m_user = v; }
    void setPassword(const QString &v) { m_password = v; }

private:
    QSqlDatabase m_db;
    QString m_lastError;

    QString m_host     = "localhost";
    int     m_port     = 5432;
    QString m_dbName   = "saturn";
    QString m_user     = "postgres";
    QString m_password = "";
};


#endif // DATAACCESS_H
