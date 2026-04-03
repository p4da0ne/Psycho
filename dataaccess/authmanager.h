#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS AuthManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY sessionChanged)
    Q_PROPERTY(int currentUserId READ currentUserId NOTIFY sessionChanged)
    Q_PROPERTY(int currentUserGroup READ currentUserGroup NOTIFY sessionChanged)
    Q_PROPERTY(QString currentUserDisplayName READ currentUserDisplayName NOTIFY sessionChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY errorChanged)

public:
    explicit AuthManager(QObject *parent = nullptr);

    static AuthManager* qmlSingleton(QQmlEngine*, QJSEngine*);
    static AuthManager* instance();

    bool loggedIn() const;
    int currentUserId() const;
    int currentUserGroup() const;
    QString currentUserDisplayName() const;
    QString lastError() const;

    Q_INVOKABLE bool login(const QString &loginName, const QString &password);
    Q_INVOKABLE void logout();

    // All current roles have full rights in the new flow.
    Q_INVOKABLE bool canRead() const;
    Q_INVOKABLE bool canCreate() const;
    Q_INVOKABLE bool canEditGeometry() const;
    Q_INVOKABLE bool canDelete() const;

signals:
    void sessionChanged();
    void errorChanged();

private:
    static QString md5Hex(const QString &value);

    static AuthManager *s_instance;

    bool m_loggedIn = false;
    int m_currentUserId = 0;
    int m_currentUserGroup = 0;
    QString m_currentUserDisplayName;
    QString m_lastError;
};

#endif // AUTHMANAGER_H
