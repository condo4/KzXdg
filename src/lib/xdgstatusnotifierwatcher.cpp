#include "xdgstatusnotifierwatcher.h"
#include <statusnotifierwatcheradaptor.h>
#include <XdgStatusNotifierItemIface.h>
#include <QDBusConnection>
#include <debug.h>

using namespace xdg;

StatusNotifierWatcher::StatusNotifierWatcher(QObject *parent)
    : QObject{parent}
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.isConnected()) {
        qCWarning(SYSTEM_TRAY) << "Cannot connect to the D-Bus session bus.\n" << "Please check your system settings and try again.\n";
    }
    if(!bus.registerService("org.kde.StatusNotifierWatcher"))
    {
        qCWarning(SYSTEM_TRAY) << "Don't register StatusNotifierWatcher service, probably already exists";
        return;
    }
    qCDebug(SYSTEM_TRAY) << "Register StatusNotifierWatcher service";
    new StatusNotifierWatcherAdaptor(this);
    bus.registerObject("/StatusNotifierWatcher", this);

    m_serviceWatcher = new QDBusServiceWatcher(this);
    m_serviceWatcher->setConnection(bus);
    m_serviceWatcher->setWatchMode(QDBusServiceWatcher::WatchForUnregistration);

    connect(m_serviceWatcher, &QDBusServiceWatcher::serviceUnregistered, this, &StatusNotifierWatcher::_serviceUnregistered);
}

StatusNotifierWatcher::~StatusNotifierWatcher()
{

}

void StatusNotifierWatcher::RegisterStatusNotifierItem(const QString &service)
{
    QString path;
    path = QStringLiteral("/StatusNotifierItem");
    qCDebug(SYSTEM_TRAY) << "Watcher: RegisterStatusNotifierItem(" << service << ")";
    QString notifierItemId = service + path;
    if (m_items.contains(notifierItemId)) {
        return;
    }
    m_serviceWatcher->addWatchedService(service);
    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(service).value()) {
        // check if the service has registered a SystemTray object
        org::kde::StatusNotifierItem trayclient(service, path, QDBusConnection::sessionBus());
        if (trayclient.isValid()) {
            qDebug() << "Registering" << notifierItemId << "to system tray";
            m_items.append(notifierItemId);
            emit StatusNotifierItemRegistered(notifierItemId);
        } else {
            m_serviceWatcher->removeWatchedService(service);
        }
    } else {
        m_serviceWatcher->removeWatchedService(service);
    }
}

void StatusNotifierWatcher::RegisterStatusNotifierHost(const QString &service)
{
    qCDebug(SYSTEM_TRAY) << "Watcher: RegisterStatusNotifierHost(" << service << ")";
    if (m_hosts.contains(service)) {
        return;
    }
    m_serviceWatcher->addWatchedService(service);
    m_hosts.append(service);
}

QStringList StatusNotifierWatcher::RegisteredStatusNotifierItems() const
{
    return m_items;
}


bool StatusNotifierWatcher::IsStatusNotifierHostRegistered() const
{
    return m_hosts.size() > 0;
}


int StatusNotifierWatcher::ProtocolVersion() const
{
    return XDG_STATUS_NOTIFIER_WATCHER_PROTOCOL_VERSION;
}

void StatusNotifierWatcher::_serviceUnregistered(const QString &name)
{
    qCDebug(SYSTEM_TRAY) << "Service " << name << "unregistered (ITEMS:" << m_items << " HOSTS:" << m_hosts << ")";
    m_serviceWatcher->removeWatchedService(name);

    const QString match = name + QLatin1Char('/');
    QStringList::Iterator it = m_items.begin();
    while (it != m_items.end()) {
        if (it->startsWith(match)) {
            QString name = *it;
            it = m_items.erase(it);
            emit StatusNotifierItemUnregistered(name);
        } else {
            ++it;
        }
    }

    it = m_hosts.begin();
    while (it != m_hosts.end()) {
        if (it->startsWith(match)) {
            QString name = *it;
            it = m_hosts.erase(it);
            emit StatusNotifierHostUnregistered();
        } else {
            ++it;
        }
    }
}
