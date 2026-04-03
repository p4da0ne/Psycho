#include "authmanager.h"

#include <QCryptographicHash>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "dataaccess.h"

AuthManager *AuthManager::s_instance = nullptr;

AuthManager::AuthManager(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

AuthManager *AuthManager::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new AuthManager();
    }
    return s_instance;
}

AuthManager *AuthManager::instance()
{
    if (s_instance == nullptr) {
        s_instance = new AuthManager();
    }
    return s_instance;
}

bool AuthManager::loggedIn() const
{
    return m_loggedIn;
}

int AuthManager::currentUserId() const
{
    return m_currentUserId;
}

int AuthManager::currentUserGroup() const
{
    return m_currentUserGroup;
}

QString AuthManager::currentUserDisplayName() const
{
    return m_currentUserDisplayName;
}

QString AuthManager::lastError() const
{
    return m_lastError;
}

bool AuthManager::login(const QString &loginName, const QString &password)
{
    m_lastError.clear();
    emit errorChanged();

    if (loginName.trimmed().isEmpty()) {
        m_lastError = "Логин не может быть пустым";
        emit errorChanged();
        return false;
    }

    DataAccess *dbAccess = DataAccess::instance();
    if (!dbAccess->connected() && !dbAccess->connectToDatabase()) {
        m_lastError = dbAccess->lastError();
        emit errorChanged();
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT u.id_user, u.id_group, u.surname, u.name, u.patronumic, up.passwd "
        "FROM users u "
        "JOIN users_passwd up ON up.id_user = u.id_user "
        "WHERE u.login_name = :login_name "
        "LIMIT 1");
    query.bindValue(":login_name", loginName);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        emit errorChanged();
        return false;
    }

    if (!query.next()) {
        m_lastError = "Неверный логин или пароль";
        emit errorChanged();
        return false;
    }

    const QString passwordHash = query.value("passwd").toString();
    if (passwordHash != md5Hex(password)) {
        m_lastError = "Неверный логин или пароль";
        emit errorChanged();
        return false;
    }

    m_loggedIn = true;
    m_currentUserId = query.value("id_user").toInt();
    m_currentUserGroup = query.value("id_group").toInt();
    m_currentUserDisplayName = QString("%1 %2 %3")
            .arg(query.value("surname").toString())
            .arg(query.value("name").toString())
            .arg(query.value("patronumic").toString())
            .trimmed();
    emit sessionChanged();
    return true;
}

void AuthManager::logout()
{
    m_loggedIn = false;
    m_currentUserId = 0;
    m_currentUserGroup = 0;
    m_currentUserDisplayName.clear();
    emit sessionChanged();
}

bool AuthManager::canRead() const
{
    return m_loggedIn;
}

bool AuthManager::canCreate() const
{
    return m_loggedIn;
}

bool AuthManager::canEditGeometry() const
{
    return m_loggedIn;
}

bool AuthManager::canDelete() const
{
    return m_loggedIn;
}

QString AuthManager::md5Hex(const QString &value)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(value.toLatin1());
    return QString(hash.result().toHex());
}
