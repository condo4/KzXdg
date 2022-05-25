#include "xdgnotifier.h"
#include "NotificationsIface.h"
#include <QGuiApplication>

XdgNotifier::XdgNotifier(QObject *parent)
    : QObject{parent}
{   
    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.\n"
                 "Please check your system settings and try again.\n");
    }
    const QString service = "org.freedesktop.Notifications";
    const QString path = "/org/freedesktop/Notifications";

    m_iface = new OrgFreedesktopNotificationsInterface(service, path, connection);

    QObject::connect(m_iface, &OrgFreedesktopNotificationsInterface::ActionInvoked, [this](uint id, const QString &action_key){
        qDebug() << "Recive signal ActionInvoked(" << id << ", " << action_key << ")";
        emit actionInvoked(action_key);
    });

    QObject::connect(m_iface, &OrgFreedesktopNotificationsInterface::NotificationClosed, [this](uint id, uint reason){
        qDebug() << "Recive signal NotificationClosed(" << id << ", " << reason << ")";
        switch(reason)
        {
        case 1:
            emit notificationClosed("The notification expired.");
            break;
        case 2:
            emit notificationClosed("The notification was dismissed by the user.");
            break;
        case 3:
            emit notificationClosed(" The notification was closed by a call to CloseNotification.");
            break;
        default:
            emit notificationClosed("Undefined/reserved reasons.");
            break;
        }
    });
}

void XdgNotifier::notifyTextOnly(QString summary, QString body, bool replace)
{
    qDebug() << "notifyTextOnly(" << summary << ", " << body << ")";
    auto reply = m_iface->Notify(
                QGuiApplication::applicationDisplayName(),
                (replace)?(m_lastid):(0),
                QString(),
                summary,
                body,
                QStringList(),
                QVariantMap(),
                -1
                );
    reply.waitForFinished();
    if(!replace)
        m_lastid = reply.value();
}

void XdgNotifier::notifyWithActions(QString summary, QString body, bool replace, QStringList actions)
{
    auto reply = m_iface->Notify(
                QGuiApplication::applicationDisplayName(),
                (replace)?(m_lastid):(0),
                QString(),
                summary,
                body,
                actions,
                QVariantMap(),
                -1
                );
    reply.waitForFinished();
    if(!replace)
        m_lastid = reply.value();
}

void XdgNotifier::notify(const QString &summary, const QString &body, bool replace, const QStringList &actions, int timeout)
{
    auto reply = m_iface->Notify(
                QGuiApplication::applicationDisplayName(),
                (replace)?(m_lastid):(0),
                QString(),
                summary,
                body,
                actions,
                QVariantMap(),
                timeout
                );
    reply.waitForFinished();
    if(!replace)
        m_lastid = reply.value();
}

void XdgNotifier::closeNotification()
{
    auto reply = m_iface->CloseNotification(m_lastid);
}
