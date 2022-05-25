#ifndef XDGNOTIFICATIONSERVER_H
#define XDGNOTIFICATIONSERVER_H

#include <QObject>

typedef uint (*NotifyCallback)(QObject *obj, const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout) override;


class XdgNotificationServer : public QObject
{
    Q_OBJECT

    NotifyCallback m_notify;
    QObject *m_notifyData;

public:
    explicit XdgNotificationServer(QObject *parent = nullptr);
    static void RegisterServerToDbus(XdgNotificationServer &server);

    void setNotify(NotifyCallback newNotify, QObject *obj);
    void sendActionInvoked(uint id, const QString &action_key);
    void sendNotificationClosed(uint id, uint reason);

public slots:

    /* DBus API */
    virtual void CloseNotification(uint id);
    virtual QStringList GetCapabilities();
    virtual QString GetServerInformation(QString &vendor, QString &version, QString &spec_version);
    virtual uint Notify(const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout);



signals:
    void closeNotification(uint id);

    void ActionInvoked(uint id, const QString &action_key);
    void NotificationClosed(uint id, uint reason);

};

#endif // XDGNOTIFICATIONSERVER_H
