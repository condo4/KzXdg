#include "xdgstatusnotifierhost.h"
#include "XdgStatusNotifierItemIface.h"
#include "XdgStatusNotifierWatcherIface.h"
#include <QDBusConnection>
#include "debug.h"

using namespace org::kde;

namespace KaZoe {
    class StatusNotifierHostPrivate {
        Q_DISABLE_COPY(StatusNotifierHostPrivate)
        Q_DECLARE_PUBLIC(StatusNotifierHost)
        StatusNotifierHost * const q_ptr;
        StatusNotifierHostPrivate(StatusNotifierHost *parent)
            : q_ptr(parent)
            , m_bus(QDBusConnection::sessionBus())
            {};

        QMap<QString, StatusNotifierItem*> m_items;
        QList<QString> m_itemsorder;
        StatusNotifierWatcher *m_watcher;
        QDBusConnection m_bus;
        QString m_serviceName;

        void addNotifier(QString item)
        {
            StatusNotifierItem *stat = new StatusNotifierItem(item.split("/").first(), "/StatusNotifierItem", m_bus, q_ptr);
            int index = m_itemsorder.size();
            q_ptr->beginInsertRows(QModelIndex(), index, index);
            m_itemsorder.append(item);
            m_items[item] = stat;

            QObject::connect(stat, &StatusNotifierItem::NewAttentionIcon, [index, this](){
                QVector<int> r;
                r.append(StatusNotifierHost::AttentionIconNameRole);
                emit q_ptr->dataChanged(q_ptr->index(index), q_ptr->index(index), r);
            });

            QObject::connect(stat, &StatusNotifierItem::NewIcon, [index, this](){
                QVector<int> r;
                r.append(StatusNotifierHost::IconNameRole);
                emit q_ptr->dataChanged(q_ptr->index(index), q_ptr->index(index), r);
            });

            QObject::connect(stat, &StatusNotifierItem::NewOverlayIcon, [index, this](){
                QVector<int> r;
                r.append(StatusNotifierHost::OverlayIconNameRole);
                emit q_ptr->dataChanged(q_ptr->index(index), q_ptr->index(index), r);
            });

            QObject::connect(stat, &StatusNotifierItem::NewStatus, [index, this](){
                QVector<int> r;
                r.append(StatusNotifierHost::StatusRole);
                emit q_ptr->dataChanged(q_ptr->index(index), q_ptr->index(index), r);
            });

            QObject::connect(stat, &StatusNotifierItem::NewTitle, [index, this](){
                QVector<int> r;
                r.append(StatusNotifierHost::TitleRole);
                emit q_ptr->dataChanged(q_ptr->index(index), q_ptr->index(index), r);
            });

            q_ptr->endInsertRows();
        }

        void removeNotifier(QString item)
        {
            int index = m_itemsorder.indexOf(item);
            q_ptr->beginRemoveRows(QModelIndex(), index, index);
            m_itemsorder.removeAll(item);
            m_items[item]->deleteLater();
            m_items.remove(item);
            q_ptr->endRemoveRows();
        }
    };
};

using namespace KaZoe;

StatusNotifierHost::StatusNotifierHost(QObject *parent)
    : QAbstractListModel{parent}
    , d_ptr(new StatusNotifierHostPrivate(this))
{
    Q_D(StatusNotifierHost);

    d->m_serviceName = "org.kde.StatusNotifierHost-";
    d->m_serviceName.append(QString::number(QCoreApplication::applicationPid()));
    qCDebug(SYSTEM_TRAY) << "Register Host service" << d->m_serviceName << ":" << d->m_bus.registerService(d->m_serviceName);

    d->m_watcher = new StatusNotifierWatcher("org.kde.StatusNotifierWatcher", "/StatusNotifierWatcher", d->m_bus, this);
    qCDebug(SYSTEM_TRAY) << "Find Watcher " << d->m_watcher->isValid() << " with protocol " << d->m_watcher->protocolVersion();

    if(!d->m_watcher->isValid())
    {
        qCWarning(SYSTEM_TRAY) << "Can't find watcher org.kde.StatusNotifierWatcher/StatusNotifierWatcher";
    }


    d->m_watcher->RegisterStatusNotifierHost(d->m_serviceName);

    QObject::connect(d->m_watcher, &StatusNotifierWatcher::StatusNotifierItemRegistered, [this](QString in0){
        Q_D(StatusNotifierHost);
        d->addNotifier(in0);
    });

    QObject::connect(d->m_watcher, &StatusNotifierWatcher::StatusNotifierItemUnregistered, [this](QString in0){
        Q_D(StatusNotifierHost);
        d->removeNotifier(in0);
    });

    for(QString item: d->m_watcher->registeredStatusNotifierItems())
    {
        d->addNotifier(item);
    }
}

StatusNotifierHost::~StatusNotifierHost() = default;

int StatusNotifierHost::rowCount(const QModelIndex &parent) const
{
    Q_D(const StatusNotifierHost);
    return d->m_items.size();
}

QVariant StatusNotifierHost::data(const QModelIndex &index, int role) const
{
    Q_D(const StatusNotifierHost);

    if(index.row() > d->m_itemsorder.count())
        return QVariant();

    const StatusNotifierItem *stat =  d->m_items[d->m_itemsorder[index.row()]];

    switch(role) {
    case CategoryRole:
        return stat->category();
    case IdRole:
        return stat->id();
    case TitleRole:
        return stat->title();
    case StatusRole:
        return stat->status();
    case WindowIdRole:
        return stat->windowId();
    case IconNameRole:
        return stat->iconName();
    case OverlayIconNameRole:
        return stat->overlayIconName();
    case AttentionIconNameRole:
        return stat->attentionIconName();
    case AttentionMovieNameRole:
        return stat->attentionMovieName();
    }

    return QVariant();
}

QHash<int, QByteArray> StatusNotifierHost::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[CategoryRole] = "category";
    roles[IdRole] = "idItem";
    roles[TitleRole] = "title";
    roles[StatusRole] = "status";
    roles[WindowIdRole] = "windowId";
    roles[IconNameRole] = "iconName";
    roles[OverlayIconNameRole] = "overlayIconName";
    roles[AttentionIconNameRole] = "attentionIconName";
    roles[AttentionMovieNameRole] = "attentionMovieName";
    return roles;
}

void StatusNotifierHost::activate(int x, int y, QString id, int mode)
{
    Q_D(const StatusNotifierHost);
    for(const QString &key: d->m_itemsorder)
    {
        if(d->m_items[key]->id() == id)
        {
            if(mode == 1)
                d->m_items[key]->SecondaryActivate(x, y);
            else
                d->m_items[key]->Activate(x, y);
            return;
        }
    }
}
