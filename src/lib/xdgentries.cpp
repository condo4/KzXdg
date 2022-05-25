#include "xdgentries.h"
#include "xdgentry.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <set>
#include <algorithm>

KaZoe::Entries::Entries(QObject *parent)
    : QObject(parent)
{
    QObject::connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &Entries::_directoryChanged, Qt::QueuedConnection);
}

void KaZoe::Entries::_directoryChanged(const QString &path)
{
    QDir directory(path);
    QStringList old, now, removed, added;
    for(KaZoe::Entry *entry: m_entries)
    {
        if(entry->path().startsWith(directory.absolutePath()))
        {
            old.append(entry->path());
        }
    }
    for(const QString &item: directory.entryList(QStringList() << "*.desktop", QDir::Files))
    {
        QString current = directory.absoluteFilePath(item);
        if(current.startsWith(directory.absolutePath()))
        {
            now.append(current);
        }
    }
    std::set_difference(old.begin(), old.end(), now.begin(), now.end(), std::inserter(removed, removed.begin()));
    std::set_difference(now.begin(), now.end(), old.begin(), old.end(), std::inserter(added, added.begin()));
    for(const QString &item: removed)
    {
        unregister(item);
    }
    for(const QString &item: added)
    {
        add(item, true);
    }
}

void KaZoe::Entries::_entryDataChanged(const QString key, const QString value)
{
    KaZoe::Entry* ent = qobject_cast<KaZoe::Entry*>(QObject::sender());
    emit entryDataChanged(ent->appId(), key, value);
}

void KaZoe::Entries::unregister(const QString &path)
{
    QFileInfo info(path);
    QString id = info.completeBaseName();
    int oldid = m_entries.keys().indexOf(id);
    emit startRemoveEntry(oldid);
    KaZoe::Entry* old = m_entries[id];
    m_watcher.removePath(old->path());
    m_entries.remove(id);
    emit endRemoveEntry(oldid);
}

int KaZoe::Entries::add(const QString &path, bool replace)
{
    QFileInfo info(path);
    QString id = info.completeBaseName();
    if(!info.isFile() || info.suffix() != "desktop")
    {
        qWarning() << path << "is not a desktop entry";
        return -1;
    }

    if(m_entries.contains(id))
    {
        if(replace)
        {
            unregister(path);
        }
        else
        {
            return -1;
        }
    }

    KaZoe::Entry* entry = new KaZoe::Entry(path);
    QObject::connect(entry, &KaZoe::Entry::dataChanged, this, &Entries::_entryDataChanged);
    QObject::connect(entry, &KaZoe::Entry::raiseProcess, this, &Entries::raiseProcess);

    m_entries[id] = entry;
    emit startCreateEntry(m_entries.keys().indexOf(id));
    emit endCreateEntry(m_entries.keys().indexOf(id));
    return 0;
}

int KaZoe::Entries::addDirectory(const QString &path, bool replace)
{
    QFileInfo info(path.trimmed());
    if(!info.isDir())
    {
        return -1;
    }

    QDir directory(path.trimmed());
    QStringList items = directory.entryList(QStringList() << "*.desktop", QDir::Files);
    for(QString &item: items)
    {
        add(directory.absoluteFilePath(item), replace);
    }

    m_watcher.addPath(path.trimmed());
    return 0;
}

qsizetype KaZoe::Entries::count() const
{
    return m_entries.count();
}

const KaZoe::Entry *KaZoe::Entries::getEntry(int i) const
{
    return m_entries[m_entries.keys()[i]];
}

QList<KaZoe::Entry *> KaZoe::Entries::getEntries()
{
    return m_entries.values();
}
