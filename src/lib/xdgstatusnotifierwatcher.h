#ifndef XDGSTATUSNOTIFIERWATCHER_H
#define XDGSTATUSNOTIFIERWATCHER_H

#include <QObject>

class QDBusServiceWatcher;
#define XDG_STATUS_NOTIFIER_WATCHER_PROTOCOL_VERSION 1

namespace xdg {

    class StatusNotifierWatcher: public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QStringList RegisteredStatusNotifierItems READ RegisteredStatusNotifierItems NOTIFY RegisteredStatusNotifierItemsChanged)
        Q_PROPERTY(bool IsStatusNotifierHostRegistered READ IsStatusNotifierHostRegistered NOTIFY IsStatusNotifierHostRegisteredChanged)
        Q_PROPERTY(int ProtocolVersion READ ProtocolVersion CONSTANT)

    public:
        StatusNotifierWatcher(QObject *parent = nullptr);
        virtual ~StatusNotifierWatcher();

        void RegisterStatusNotifierItem(const QString &service);
        void RegisterStatusNotifierHost(const QString &service);

        QStringList RegisteredStatusNotifierItems() const;
        bool IsStatusNotifierHostRegistered() const;
        int ProtocolVersion() const;

    signals:
        void StatusNotifierItemRegistered(QString id);
        void StatusNotifierItemUnregistered(QString id);
        void StatusNotifierHostRegistered();
        void StatusNotifierHostUnregistered();
        void RegisteredStatusNotifierItemsChanged();
        void IsStatusNotifierHostRegisteredChanged();

    private:
        QStringList m_hosts;
        QStringList m_items;
        QDBusServiceWatcher *m_serviceWatcher = nullptr;

    private slots:
        void _serviceUnregistered(const QString &name);
    };
};

#endif // XDGSTATUSNOTIFIERWATCHER_H
