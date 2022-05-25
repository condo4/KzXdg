#ifndef SystrayNotifyManager_H
#define SystrayNotifyManager_H

#include <QAbstractListModel>
#include <QObject>

class XdgNotificationServer;

namespace xdg {
    uint serverNotify(QObject *manager, const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout);
    class NotificationMessage;

    class NotificationManager : public QAbstractListModel
    {
        Q_OBJECT
        Q_PROPERTY(int nbNotifications READ nbNotifications NOTIFY notificationsChanged)

        XdgNotificationServer *m_server;
        unsigned int m_last = 0;
        QMap<unsigned int, NotificationMessage *> m_pending;
        QList<unsigned int> m_ids;

        friend uint serverNotify(QObject *manager, const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout);
    public:
        explicit NotificationManager(QObject *parent = nullptr);

        enum SystrayNotifyRoles {
            AppNameRole = Qt::UserRole + 1,
            AppIconRole,
            SummaryRole,
            BodyRole,
            ActionsRole,
            HintsRole,
            IdRole,
        };

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;


    public slots:
        int nbNotifications();
        void close(unsigned int id, unsigned int reason = 2);
        void returnAction(unsigned int appid, const QString &key);

    private slots:
        void _closeNotification(unsigned int id);

    signals:
        void notificationsChanged();
    };
}

#endif // SystrayNotifyManager_H
