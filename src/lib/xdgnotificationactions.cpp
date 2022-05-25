#include "xdgnotificationactions.h"
#include <QDebug>

xdg::NotificationActions::NotificationActions(QObject *parent)
    : QAbstractListModel(parent)
{

}

QStringList xdg::NotificationActions::actionslist() const
{
    return m_actionslist;
}

void xdg::NotificationActions::setActionslist(const QStringList &newActionslist)
{
    if(newActionslist.count() % 2 != 0)
    {
        qWarning() << "Error in actionlist:" << newActionslist;
    }

    if (m_actionslist == newActionslist)
        return;
    m_actionslist = newActionslist;

    beginResetModel();
    m_actions.clear();
    for(int i = 0; i < newActionslist.count() / 2; i++)
    {
        m_actions.append(QPair<QString, QString>(newActionslist[i * 2], newActionslist[(i * 2) + 1]));
    }
    endResetModel();

    emit actionslistChanged();
}

int xdg::NotificationActions::rowCount(const QModelIndex &parent) const
{
    return m_actions.count();
}

QVariant xdg::NotificationActions::data(const QModelIndex &index, int role) const
{
    if(index.row() > m_actions.count())
        return QVariant();

    switch(role) {
        case Qt::DisplayRole:
        case TextRole:
            return m_actions[index.row()].second;
        case KeyRole:
            return m_actions[index.row()].first;
    }
    return QVariant();
}

QHash<int, QByteArray> xdg::NotificationActions::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[KeyRole] = "key";
    roles[TextRole] = "actionText";
    return roles;
}
