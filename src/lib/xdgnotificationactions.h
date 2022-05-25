#ifndef XDGNOTIFICATIONACTIONS_H
#define XDGNOTIFICATIONACTIONS_H

#include <QAbstractListModel>

namespace KaZoe {

    class NotificationActions: public QAbstractListModel
    {
        Q_OBJECT

        QList<QPair<QString, QString>> m_actions;

        Q_PROPERTY(QStringList actionslist READ actionslist WRITE setActionslist NOTIFY actionslistChanged)

    public:
        NotificationActions(QObject *parent = nullptr);

        enum NotificationActionsRoles {
            KeyRole = Qt::UserRole + 1,
            TextRole,
        };

        QStringList actionslist() const;
        void setActionslist(const QStringList &newActionslist);

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

    signals:
        void actionslistChanged();
    private:
        QStringList m_actionslist;
    };

}


#endif // XDGNOTIFICATIONACTIONS_H
