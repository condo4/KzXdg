#include "xdgnotificationserver.h"
#include <QDBusConnection>
#include <QDebug>

void XdgNotificationServer::setNotify(NotifyCallback newNotify, QObject *obj)
{
    m_notify = newNotify;
    m_notifyData = obj;
}

void XdgNotificationServer::sendActionInvoked(uint id, const QString &action_key)
{
    emit ActionInvoked(id, action_key);
}

void XdgNotificationServer::sendNotificationClosed(uint id, uint reason)
{
    emit NotificationClosed(id, reason);
}

XdgNotificationServer::XdgNotificationServer(QObject *parent)
    : QObject{parent}
{

}

void XdgNotificationServer::RegisterServerToDbus(XdgNotificationServer &server)
{
    const QString service = "org.freedesktop.Notifications";
    const QString path = "/org/freedesktop/Notifications";
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.\n"
                 "Please check your system settings and try again.\n");
    }
    bus.registerObject(path, &server);

    bool conres = bus.registerService(service);
    if(!conres) qWarning() << "Can't register dbus service " << service;
}

void XdgNotificationServer::CloseNotification(uint id)
{
    emit closeNotification(id);
}

QStringList XdgNotificationServer::GetCapabilities()
{
    return QStringList();
}

QString XdgNotificationServer::GetServerInformation(QString &vendor, QString &version, QString &spec_version)
{
    vendor = "KaZoe";
    version = "1.0.0";
    spec_version = "1.2";
    return QString(); // Why return string ? spec is return void
}

uint XdgNotificationServer::Notify(const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout)
{
    if(m_notify)
    {
        return m_notify(m_notifyData, app_name, replaces_id, app_icon, summary, body, actions, hints, timeout);
    }
    else
    {
        qDebug() << "XdgNotificationServer::Notify("
                 << app_name
                 << ", "
                 << replaces_id
                 << ", "
                 << app_icon
                   << ", "
                 << summary
                 << ", "
                 << body
                 << ", "
                 << actions
                 << ", "
                 << hints
                 << ", "
                 << timeout
                 << ")";
    }
    return 0;
}
