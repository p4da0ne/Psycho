#ifndef DBCONNECTIONSETTINGS_H
#define DBCONNECTIONSETTINGS_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QString>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS DbConnectionSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString driver READ driver WRITE setDriver NOTIFY changed)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY changed)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY changed)
    Q_PROPERTY(QString dbName READ dbName WRITE setDbName NOTIFY changed)
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY changed)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY changed)

public:
    explicit DbConnectionSettings(QObject *parent = nullptr);

    static DbConnectionSettings* qmlSingleton(QQmlEngine*, QJSEngine*);
    static DbConnectionSettings* instance();

    QString driver() const;
    QString host() const;
    int port() const;
    QString dbName() const;
    QString user() const;
    QString password() const;

    void setDriver(const QString &value);
    void setHost(const QString &value);
    void setPort(int value);
    void setDbName(const QString &value);
    void setUser(const QString &value);
    void setPassword(const QString &value);

    Q_INVOKABLE void load();
    Q_INVOKABLE void save();
    Q_INVOKABLE bool applyAndConnect();

signals:
    void changed();

private:
    static DbConnectionSettings *s_instance;

    QSettings m_settings;
    QString m_driver = "QPSQL";
    QString m_host = "localhost";
    int m_port = 5432;
    QString m_dbName = "SATURN";
    QString m_user = "postgres";
    QString m_password;
};

#endif // DBCONNECTIONSETTINGS_H
