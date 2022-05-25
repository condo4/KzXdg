#ifndef XDGDESKTOPENTRIES_H
#define XDGDESKTOPENTRIES_H

#include <QAbstractListModel>
#include <QObject>
#include "xdgentries.h"

namespace KaZoe {
    class DesktopEntries : public QAbstractListModel
    {
        Q_OBJECT
        Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
        KaZoe::Entries m_entries;

    private slots:
        bool _contains(QString path);
        //void _reload(QString path);
        QStringList _getEntriesFrom(QString path = "");
        void _dataChanged(const QString appid, const QString item, const QString value);

    public:
        explicit DesktopEntries(QObject *parent = nullptr);

        enum XdgDesktopEntriesRoles {
            PathRole = Qt::UserRole + 1,
            NameRole,
            IconRole,
            CategoryRole,
            NoDisplayRole,
            KeywordsRole,
            ExecRole,
            DesktopFileRole,
            AppIdRole,
            AppNameRole,
            EntryRole
        };

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        QStringList categories();

    public slots:
        void start(QString path, const QStringList &args = QStringList());
        void stop(QString path, const QStringList &args = QStringList());
        QString appIdData(QString appid, QString key);

    signals:
        void raiseProcess(int pid);
        void categoriesChanged(QStringList categories);

    private:
        QString m_sources;
    };
}

#endif // XDGDESKTOPENTRIES_H
