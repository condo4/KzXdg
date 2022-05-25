#include "xdgstatusnotifieritem.h"
#include <statusnotifieritemadaptor.h>
#include <XdgStatusNotifierWatcherIface.h>
#include <QDBusConnection>
#include <QGuiApplication>
#include <debug.h>

using namespace KaZoe;

StatusNotifierItem::StatusNotifierItem(QObject *parent)
    : QObject{parent}
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.isConnected()) {
        qCWarning(SYSTEM_TRAY) << "Cannot connect to the D-Bus session bus.\n" << "Please check your system settings and try again.\n";
    }
    QString serviceName = "org.kazoe.xdg";
    setId((QGuiApplication::desktopFileName().isEmpty())?(QCoreApplication::applicationName()):(QGuiApplication::desktopFileName()));
    serviceName.append(Id());
    serviceName.append("-");
    serviceName.append(QString::number(QCoreApplication::applicationPid()));
    qCDebug(SYSTEM_TRAY) << "Register Item service" << serviceName << ":" << bus.registerService(serviceName);
    new StatusNotifierItemAdaptor(this);
    bus.registerObject("/StatusNotifierItem", this);
    OrgKdeStatusNotifierWatcherInterface *watcher = new OrgKdeStatusNotifierWatcherInterface("org.kde.StatusNotifierWatcher", "/StatusNotifierWatcher", bus);
    watcher->RegisterStatusNotifierItem(serviceName);
}

QString StatusNotifierItem::Title() const
{
    return m_Title;
}

void StatusNotifierItem::setTitle(const QString &newTitle)
{
    if (m_Title == newTitle)
        return;
    m_Title = newTitle;
    emit NewTitle();
    emit TitleChanged();
}

QString StatusNotifierItem::IconName() const
{
    return m_IconName;
}

void StatusNotifierItem::setIconName(const QString &newIconName)
{
    if (m_IconName == newIconName)
        return;
    m_IconName = newIconName;
    emit IconNameChanged();
    emit NewIcon();
}


void StatusNotifierItem::Activate(uint x, uint y)
{
    emit activated(x, y);
}

void StatusNotifierItem::SecondaryActivate(uint x, uint y)
{
    emit secondaryActivated(x, y);
}

QString StatusNotifierItem::Status() const
{
    return m_Status;
}

void StatusNotifierItem::setStatus(const QString &newStatus)
{
    if (m_Status == newStatus)
        return;
    if(!xdgStatus.contains(newStatus))
    {
        qCWarning(SYSTEM_TRAY) << "Status " << newStatus << "not authorized";
        return;
    }
    m_Status = newStatus;
    emit StatusChanged();
    emit NewStatus(m_Status);
}

QString StatusNotifierItem::OverlayIconName() const
{
    return m_OverlayIconName;
}

void StatusNotifierItem::setOverlayIconName(const QString &newOverlayIconName)
{
    if (m_OverlayIconName == newOverlayIconName)
        return;
    m_OverlayIconName = newOverlayIconName;
    emit overlayIconNameChanged();
    emit NewOverlayIcon();
}

QString StatusNotifierItem::Category() const
{
    return m_Category;
}

void StatusNotifierItem::setCategory(const QString &newCategory)
{
    if (m_Category == newCategory)
        return;
    if(!xdgCategories.contains(newCategory))
    {
        qCWarning(SYSTEM_TRAY) << "Category " << newCategory << "not authorized";
        return;
    }
    m_Category = newCategory;
    emit CategoryChanged();
}

QString StatusNotifierItem::Id() const
{
    return m_Id;
}

void StatusNotifierItem::setId(const QString &newId)
{
    if (m_Id == newId)
        return;
    m_Id = newId;
    emit IdChanged();
}

QString StatusNotifierItem::WindowId() const
{
    return m_WindowId;
}

void StatusNotifierItem::setWindowId(const QString &newWindowId)
{
    if (m_WindowId == newWindowId)
        return;
    m_WindowId = newWindowId;
    emit WindowIdChanged();
}

QString StatusNotifierItem::AttentionIconName() const
{
    return m_AttentionIconName;
}

void StatusNotifierItem::setAttentionIconName(const QString &newAttentionIconName)
{
    if (m_AttentionIconName == newAttentionIconName)
        return;
    m_AttentionIconName = newAttentionIconName;
    emit AttentionIconNameChanged();
}

QString StatusNotifierItem::AttentionMovieName() const
{
    return m_AttentionMovieName;
}

void StatusNotifierItem::setAttentionMovieName(const QString &newAttentionMovieName)
{
    if (m_AttentionMovieName == newAttentionMovieName)
        return;
    m_AttentionMovieName = newAttentionMovieName;
    emit AttentionMovieNameChanged();
}
