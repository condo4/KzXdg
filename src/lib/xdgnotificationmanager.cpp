#include "xdgnotificationmanager.h"
#include "xdgnotificationserver.h"
#include "xdgnotificationmessage.h"
#include "notificationsadaptor.h"

uint xdg::serverNotify(QObject *obj, const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout)
{
    xdg::NotificationManager *manager = qobject_cast<xdg::NotificationManager*>(obj);
    if(replaces_id != 0 && manager->m_pending.contains(replaces_id))
    {
        xdg::NotificationMessage *message = manager->m_pending[replaces_id];
        message->update(app_name, app_icon, summary, body, actions, hints, timeout);
    }
    else
    {
        xdg::NotificationMessage *message = new xdg::NotificationMessage(app_name, app_icon, summary, body, actions, hints, timeout, manager);

        QObject::connect(message, &xdg::NotificationMessage::appNameChanged, [manager, replaces_id](){
            const QVector<int> roles = {xdg::NotificationManager::AppNameRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });

        QObject::connect(message, &xdg::NotificationMessage::appIconChanged, [manager, replaces_id](){
            const QVector<int> roles = {xdg::NotificationManager::AppIconRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });

        QObject::connect(message, &xdg::NotificationMessage::summaryChanged, [manager, replaces_id](){
            const QVector<int> roles = {xdg::NotificationManager::SummaryRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });

        QObject::connect(message, &xdg::NotificationMessage::bodyChanged, [manager, replaces_id](){
            const QVector<int> roles = {xdg::NotificationManager::BodyRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });

        QObject::connect(message, &xdg::NotificationMessage::actionsChanged, [manager, replaces_id](){
            const QVector<int> roles = {xdg::NotificationManager::ActionsRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });

        QObject::connect(message, &xdg::NotificationMessage::hintsChanged, [manager, replaces_id](){
            const QVector<int> roles = {xdg::NotificationManager::HintsRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });


        QObject::connect(message, &xdg::NotificationMessage::expired, [manager](QObject *obj){
            xdg::NotificationMessage *msg = qobject_cast<xdg::NotificationMessage*>(obj);
            if(msg)
            {
                manager->close(manager->m_pending.key(msg), 1);
            }
        });

        manager->beginInsertRows(QModelIndex(), 0, 0);
        manager->m_pending[++manager->m_last] = message;
        message->setId(manager->m_last);
        manager->m_ids.insert(0, manager->m_last);
        manager->endInsertRows();
    }
    return manager->m_last;
}


xdg::NotificationManager::NotificationManager(QObject *parent)
    : QAbstractListModel(parent)
{
    m_server = new XdgNotificationServer(this);
    m_server->setNotify(serverNotify, this);
    QObject::connect(m_server, &XdgNotificationServer::closeNotification, this, &NotificationManager::_closeNotification);
    QObject::connect(this, &NotificationManager::rowsRemoved, this, &NotificationManager::notificationsChanged);
    QObject::connect(this, &NotificationManager::rowsInserted, this, &NotificationManager::notificationsChanged);
    new NotificationsAdaptor(m_server);
    XdgNotificationServer::RegisterServerToDbus(*m_server);
}

int xdg::NotificationManager::rowCount(const QModelIndex &parent) const
{
    return m_ids.count();
}

QVariant xdg::NotificationManager::data(const QModelIndex &index, int role) const
{
    if(index.row() > m_ids.count())
        return QVariant();

    const NotificationMessage *message = m_pending[m_ids[index.row()]];
    if(message == nullptr)
        return QVariant();

    switch(role) {
        case Qt::DisplayRole:
        case AppNameRole:
            return message->appName();
        case AppIconRole:
            return message->appIcon();
        case SummaryRole:
            return message->summary();
        case BodyRole:
            return message->body();
        case ActionsRole:
            return message->actions();
        case HintsRole:
            return message->hints();
        case IdRole:
            return message->id();
    }
    return QVariant();
}

QHash<int, QByteArray> xdg::NotificationManager::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[AppNameRole] = "appName";
    roles[AppIconRole] = "appIcon";
    roles[SummaryRole] = "summary";
    roles[BodyRole] = "body";
    roles[ActionsRole] = "actions";
    roles[HintsRole] = "hints";
    roles[IdRole] = "appId";
    return roles;
}


int xdg::NotificationManager::nbNotifications()
{
    return rowCount();
}

void xdg::NotificationManager::close(unsigned int id, unsigned int reason)
{
    unsigned int index = m_ids.indexOf(id);
    beginRemoveRows(QModelIndex(), index, index);
    m_ids.removeAll(id);
    NotificationMessage *message = m_pending[id];
    m_pending.remove(id);
    if (message != NULL) {
        message->deleteLater();
    }
    endRemoveRows();
    m_server->sendNotificationClosed(id, reason);
}

void xdg::NotificationManager::returnAction(unsigned int appid, const QString &key)
{
    m_server->sendActionInvoked(appid, key);
    close(appid, 2);
}

void xdg::NotificationManager::_closeNotification(unsigned int id)
{
    close(id, 3);
}
