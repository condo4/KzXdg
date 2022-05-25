#include "xdgnotificationmanager.h"
#include "xdgnotificationserver.h"
#include "xdgnotificationmessage.h"
#include "notificationsadaptor.h"

uint KaZoe::serverNotify(QObject *obj, const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout)
{
    KaZoe::NotificationManager *manager = qobject_cast<KaZoe::NotificationManager*>(obj);
    if(replaces_id != 0 && manager->m_pending.contains(replaces_id))
    {
        KaZoe::NotificationMessage *message = manager->m_pending[replaces_id];
        message->update(app_name, app_icon, summary, body, actions, hints, timeout);
    }
    else
    {
        KaZoe::NotificationMessage *message = new KaZoe::NotificationMessage(app_name, app_icon, summary, body, actions, hints, timeout, manager);

        QObject::connect(message, &KaZoe::NotificationMessage::appNameChanged, [manager, replaces_id](){
            const QVector<int> roles = {KaZoe::NotificationManager::AppNameRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });

        QObject::connect(message, &KaZoe::NotificationMessage::appIconChanged, [manager, replaces_id](){
            const QVector<int> roles = {KaZoe::NotificationManager::AppIconRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });

        QObject::connect(message, &KaZoe::NotificationMessage::summaryChanged, [manager, replaces_id](){
            const QVector<int> roles = {KaZoe::NotificationManager::SummaryRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });

        QObject::connect(message, &KaZoe::NotificationMessage::bodyChanged, [manager, replaces_id](){
            const QVector<int> roles = {KaZoe::NotificationManager::BodyRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });

        QObject::connect(message, &KaZoe::NotificationMessage::actionsChanged, [manager, replaces_id](){
            const QVector<int> roles = {KaZoe::NotificationManager::ActionsRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });

        QObject::connect(message, &KaZoe::NotificationMessage::hintsChanged, [manager, replaces_id](){
            const QVector<int> roles = {KaZoe::NotificationManager::HintsRole};
            emit manager->dataChanged(
                        manager->index(replaces_id),
                        manager->index(replaces_id),
                        roles);
        });


        QObject::connect(message, &KaZoe::NotificationMessage::expired, [manager](QObject *obj){
            KaZoe::NotificationMessage *msg = qobject_cast<KaZoe::NotificationMessage*>(obj);
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


KaZoe::NotificationManager::NotificationManager(QObject *parent)
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

int KaZoe::NotificationManager::rowCount(const QModelIndex &parent) const
{
    return m_ids.count();
}

QVariant KaZoe::NotificationManager::data(const QModelIndex &index, int role) const
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

QHash<int, QByteArray> KaZoe::NotificationManager::roleNames() const
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


int KaZoe::NotificationManager::nbNotifications()
{
    return rowCount();
}

void KaZoe::NotificationManager::close(unsigned int id, unsigned int reason)
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

void KaZoe::NotificationManager::returnAction(unsigned int appid, const QString &key)
{
    m_server->sendActionInvoked(appid, key);
    close(appid, 2);
}

void KaZoe::NotificationManager::_closeNotification(unsigned int id)
{
    close(id, 3);
}
