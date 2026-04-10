#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>

#include "config_dataaccess.h"

class QQmlEngine;
class QJSEngine;

class __EXPORT_DATAACCESS DataAccess : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectionChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY errorOccurred)
    Q_PROPERTY(QString driver READ driver WRITE setDriver NOTIFY settingsChanged)
    Q_PROPERTY(QString host WRITE setHost NOTIFY settingsChanged)
    Q_PROPERTY(int port WRITE setPort NOTIFY settingsChanged)
    Q_PROPERTY(QString dbName WRITE setDbName NOTIFY settingsChanged)
    Q_PROPERTY(QString user WRITE setUser NOTIFY settingsChanged)
    Q_PROPERTY(QString password WRITE setPassword NOTIFY settingsChanged)

public:
    explicit DataAccess(QObject *parent = nullptr);
    ~DataAccess();

    static DataAccess* qmlSingleton(QQmlEngine*, QJSEngine*);
    static DataAccess* instance();

    bool connected() const;
    QString lastError() const;
    QString driver() const;
    QString host() const;
    int port() const;
    QString dbName() const;
    QString user() const;
    QString password() const;

    Q_INVOKABLE bool connectToDatabase();
    Q_INVOKABLE bool connect();
    Q_INVOKABLE bool disconnectDatabase();

    void setDriver(const QString &v);
    void setHost(const QString &v);
    void setPort(int v);
    void setDbName(const QString &v);
    void setUser(const QString &v);
    void setPassword(const QString &v);

    // Legacy compatibility API used by existing widgets modules.
    bool create_connection(QString db_driver, QString database, QString user, QString user_password, QString hostname, int port = 5432);
    bool get_connection_flag();
    QString get_message();
    bool close_connection_();
    QString get_user();
    QString get_user_password();
    QString get_hostname();
    void set_user(QString new_user);
    void set_user_password(QString new_user_password);
    void set_hostname(QString new_hostname);

signals:
    void connectionChanged();
    void errorOccurred();
    void settingsChanged();

private:
    static QString defaultConnectionName();
    static DataAccess *s_instance;

    QSqlDatabase ensureDatabase();

    QSqlDatabase m_db;
    QString m_lastError;
    bool m_connectionFlag = false;

    QString m_driver = "QPSQL";
    QString m_host = "localhost";
    int m_port = 5432;
    QString m_dbName = "saturn";
    QString m_user = "postgres";
    QString m_password = "";
};

#endif // DATAACCESS_H
