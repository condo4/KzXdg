#ifndef XDGSTATUSNOTIFIERHOST_H
#define XDGSTATUSNOTIFIERHOST_H

#include <QAbstractListModel>
#include <QObject>
#include <QScopedPointer>

namespace xdg {
    class StatusNotifierHostPrivate;

    class StatusNotifierHost : public QAbstractListModel
    {
        Q_OBJECT
    public:
        explicit StatusNotifierHost(QObject *parent = nullptr);
        virtual ~StatusNotifierHost();

        enum XdgDesktopEntriesRoles {
            CategoryRole = Qt::UserRole + 1,
            IdRole,
            TitleRole,
            StatusRole,
            WindowIdRole,
            IconNameRole,
            OverlayIconNameRole,
            AttentionIconNameRole,
            AttentionMovieNameRole
        };

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

    public slots:
        void activate(int x, int y, QString id, int mode);

    private:
        Q_DISABLE_COPY(StatusNotifierHost)
        Q_DECLARE_PRIVATE(StatusNotifierHost)
        QScopedPointer<StatusNotifierHostPrivate> const d_ptr;
    };
};

#endif // XDGSTATUSNOTIFIERHOST_H
