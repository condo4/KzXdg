#include "xdgnotificationmessage.h"
#include <QTimer>


QString KaZoe::NotificationMessage::appName() const
{
    return m_appName;
}

QString KaZoe::NotificationMessage::appIcon() const
{
    return m_appIcon;
}

QString KaZoe::NotificationMessage::summary() const
{
    return m_summary;
}

QString KaZoe::NotificationMessage::body() const
{
    return m_body;
}

QStringList KaZoe::NotificationMessage::actions() const
{
    return m_actions;
}

QVariantMap KaZoe::NotificationMessage::hints() const
{
    return m_hints;
}

uint KaZoe::NotificationMessage::id() const
{
    return m_id;
}

void KaZoe::NotificationMessage::_expired()
{
    emit expired(this);
}


KaZoe::NotificationMessage::NotificationMessage(
        const QString &app_name,
        const QString &app_icon,
        const QString &summary,
        const QString &body,
        const QStringList &actions,
        const QVariantMap &hints,
        int timeout,
        QObject *parent)
    : QObject(parent)
    , m_appName(app_name)
    , m_appIcon(app_icon)
    , m_summary(summary)
    , m_body(body)
    , m_actions(actions)
    , m_hints(hints)
    , m_timeout(timeout)
{
    if(m_timeout > 0)
        QTimer::singleShot(m_timeout, this, &KaZoe::NotificationMessage::_expired);
}

void KaZoe::NotificationMessage::update(const QString &app_name, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout)
{
    if(m_appName != app_name)
    {
        m_appName = app_name;
        emit appNameChanged(m_appName);
    }

    if(m_appIcon != app_icon)
    {
        m_appIcon = app_icon;
        emit appIconChanged(m_appIcon);
    }

    if(m_summary != summary)
    {
        m_summary = summary;
        emit summaryChanged(m_summary);
    }

    if(m_body != app_name)
    {
        m_body = body;
        emit bodyChanged(m_body);
    }

    if(m_actions != actions)
    {
        m_actions = actions;
        emit actionsChanged(m_actions);
    }

    if(m_hints != hints)
    {
        m_hints = hints;
        emit hintsChanged(m_hints);
    }

    if(m_timeout != timeout)
    {
        m_timeout = timeout;
        emit timeoutChanged(m_timeout);
    }
    if(m_timeout > 0)
        QTimer::singleShot(m_timeout, this, &KaZoe::NotificationMessage::_expired);

}

void KaZoe::NotificationMessage::setId(uint id) 
{
    m_id = id;
}
