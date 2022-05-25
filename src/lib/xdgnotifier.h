#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>
#include <QDBusConnection>

class OrgFreedesktopNotificationsInterface;

class XdgNotifier : public QObject
{
    Q_OBJECT
    OrgFreedesktopNotificationsInterface  *m_iface;
    int m_lastid;
public:
    explicit XdgNotifier(QObject *parent = nullptr);

public slots:
    void notifyTextOnly(QString summary, QString body, bool replace);
    void notifyWithActions(QString summary, QString body, bool replace, QStringList actions);
    void notify(const QString &summary, const QString &body, bool replace, const QStringList &actions, int timeout);
    void closeNotification();

signals:
    void actionInvoked(QString action);
    void notificationClosed(QString reason);
};

#endif // NOTIFIER_H
