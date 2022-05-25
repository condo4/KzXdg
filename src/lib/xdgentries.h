#ifndef XDGENTRIES_H
#define XDGENTRIES_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QMap>

namespace xdg {
    class Entry;

    class Entries : public QObject
    {
        Q_OBJECT
        QString m_type;
        QFileSystemWatcher m_watcher;
        QMap<QString, xdg::Entry*> m_entries;

    private slots:
        void _directoryChanged(const QString &path);
        void _entryDataChanged(const QString key, const QString value);

    public:
        explicit Entries(QObject *parent = nullptr);
        void unregister(const QString &path);
        int add(const QString &path, bool replace = false);
        int addDirectory(const QString &path, bool replace = false);
        qsizetype count() const;
        const Entry *getEntry(int i) const;
        QList<xdg::Entry *> getEntries();

    signals:
        void entryDataChanged(const QString appid, const QString key, const QString value);
        void raiseProcess(int pid);
        void startCreateEntry(int id);
        void endCreateEntry(int id);
        void startRemoveEntry(int id);
        void endRemoveEntry(int id);
    };
}

#endif // XDGENTRIES_H
