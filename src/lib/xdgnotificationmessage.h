#ifndef NOTIFYMESSAGE_H
#define NOTIFYMESSAGE_H

#include <QObject>
#include <QVariantMap>

namespace xdg {
    class NotificationMessage : public QObject
    {
        Q_OBJECT

        QString m_appName;
        QString m_appIcon;
        QString m_summary;
        QString m_body;
        QStringList m_actions;
        QVariantMap m_hints;
        uint m_id;
        int m_timeout;


    public:
        explicit NotificationMessage(
                const QString &app_name,
                const QString &app_icon,
                const QString &summary,
                const QString &body,
                const QStringList &actions,
                const QVariantMap &hints,
                int timeout,
                QObject *parent = nullptr);

        void update(
                const QString &app_name,
                const QString &app_icon,
                const QString &summary,
                const QString &body,
                const QStringList &actions,
                const QVariantMap &hints,
                int timeout);

        void setId(uint id);

        QString appName() const;
        QString appIcon() const;
        QString summary() const;
        QString body() const;
        QStringList actions() const;
        QVariantMap hints() const;
        uint id() const;

    private slots:
        void _expired();

    signals:
        void appNameChanged(QString appName);
        void appIconChanged(QString appIcon);
        void summaryChanged(QString summary);
        void bodyChanged(QString body);
        void actionsChanged(QStringList actions);
        void hintsChanged(QVariantMap hints);
        void timeoutChanged(int timeout);
        void expired(QObject *obj);
    };
}

#endif // NOTIFYMESSAGE_H
