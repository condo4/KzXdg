#include "xdgdesktopentries.h"
#include "xdgentry.h"
#include "xdgbasedir.h"
#include <QDir>
#include <QMutexLocker>
#include <qdebug.h>
#include <signal.h>



KaZoe::DesktopEntries::DesktopEntries(QObject *parent)
    : QAbstractListModel(parent)
{
    QStringList datadirs = KaZoe::dataDirs();
    for (auto dir = datadirs.rbegin(); dir != datadirs.rend(); ++dir)
    {
        m_entries.addDirectory(*dir + "/applications");
    }
    m_entries.addDirectory(KaZoe::dataHome() + "/applications");
    QObject::connect(&m_entries, &KaZoe::Entries::entryDataChanged, this, &KaZoe::DesktopEntries::_dataChanged);
    QObject::connect(&m_entries, &KaZoe::Entries::raiseProcess, this, &KaZoe::DesktopEntries::raiseProcess);
    QObject::connect(&m_entries, &KaZoe::Entries::startCreateEntry, [this](int i){
        beginInsertRows(QModelIndex(), i, i);
    });
    QObject::connect(&m_entries, &KaZoe::Entries::startCreateEntry, [this](int i){
        endInsertRows();
        emit categoriesChanged(categories());
    });

    QObject::connect(&m_entries, &KaZoe::Entries::startRemoveEntry, [this](int i){
        beginRemoveRows(QModelIndex(), i, i);
    });
    QObject::connect(&m_entries, &KaZoe::Entries::startRemoveEntry, [this](int i){
        endRemoveRows();
        emit categoriesChanged(categories());
    });
}

int KaZoe::DesktopEntries::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_entries.count();
}

QVariant KaZoe::DesktopEntries::data(const QModelIndex &index, int role) const
{
    if(index.row() > m_entries.count())
        return QVariant();

    const KaZoe::Entry *entry = m_entries.getEntry(index.row());

    switch(role) {
        case Qt::DisplayRole:
        case AppNameRole:
        case NameRole:
            return entry->data(XDG_NAME);
        case ExecRole:
            return entry->data(XDG_EXEC);
        case IconRole:
            return entry->data(XDG_ICON);
        case CategoryRole:
            return entry->data(XDG_CATEGORIES);
        case NoDisplayRole:
            return entry->data(XDG_NODISPLAY);
        case KeywordsRole:
            return entry->data(XDG_KEYWORDS);
        case PathRole:
        case DesktopFileRole:
            return entry->path();
        case AppIdRole:
            return entry->appId();
        case EntryRole:
        {
            QVariant ventry;
            ventry.setValue(entry);
            return ventry;
        }
    }
    qWarning() << "Not managed role" << role;
    return QVariant();
}

QHash<int, QByteArray> KaZoe::DesktopEntries::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[IconRole] = "appIcon";
    roles[NameRole] = "name";
    roles[AppNameRole] = "appName";
    roles[ExecRole] = "exec";
    roles[PathRole] = "path";
    roles[NoDisplayRole] = "noDisplay";
    roles[CategoryRole] = "category";
    roles[KeywordsRole] = "keywords";
    roles[DesktopFileRole] = "desktopFile";
    roles[AppIdRole] = "appId";
    roles[EntryRole] = "entry";
    return roles;
}

QStringList KaZoe::DesktopEntries::categories()
{
    QStringList ret;
    for(KaZoe::Entry *entry: m_entries.getEntries())
    {
        QString c = entry->data(XDG_CATEGORIES);
        if(!ret.contains(c) && c.length())
            ret.append(c);
    }
    return ret;
}

void KaZoe::DesktopEntries::start(QString path, const QStringList& args)
{
    for(KaZoe::Entry *entry: m_entries.getEntries())
    {
        if(entry->path() == path)
            entry->start(args);
    }
}

void KaZoe::DesktopEntries::stop(QString path, const QStringList& args)
{
    bool isPid;
    int pid = path.toInt(&isPid, 10);
    for(KaZoe::Entry *entry: m_entries.getEntries())
    {
        if((isPid && entry->pid() == pid) || (entry->path() == path))
        {
            entry->stop(args);
            return;
        }
    }
    if(isPid) kill(pid, SIGTERM);
}

QString KaZoe::DesktopEntries::appIdData(QString appid, QString key)
{
    for(KaZoe::Entry *entry: m_entries.getEntries())
    {
        if(entry->appId() == appid)
            return entry->data(key);
    }
    return QString();
}

bool KaZoe::DesktopEntries::_contains(QString path)
{
    for(const KaZoe::Entry *entry: m_entries.getEntries())
    {
        if(entry->path() == path)
            return true;
    }
    return false;
}

QStringList KaZoe::DesktopEntries::_getEntriesFrom(QString path)
{
    QStringList entries;
    for(KaZoe::Entry *entry: m_entries.getEntries())
    {
        if(path == "" || entry->path().startsWith(path))
            entries.append(entry->path());
    }
    return entries;
}

void KaZoe::DesktopEntries::_dataChanged(const QString appid, const QString item, const QString value)
{
    if(item == XDG_CATEGORIES)
    {
        emit categoriesChanged(categories());
    }

    int role = -1;
    if (item == XDG_ICON)
        role = IconRole;
    else if (item == XDG_NAME)
        role = NameRole;
    else if(item == XDG_EXEC)
        role = ExecRole;
    else if (item == XDG_CATEGORIES)
        role = CategoryRole;
    else if (item == XDG_KEYWORDS)
        role = KeywordsRole;
    else if (item == XDG_NODISPLAY)
        role = NoDisplayRole;
    else
        qWarning() << "XdgEntries::_dataChanged: Unknwon role " << item;
    if(role > 0)
    {
        for(int i = 0; i < m_entries.count(); i++)
        {
            const KaZoe::Entry *entry = m_entries.getEntry(i);
            if(entry->appId() == appid)
            {
                QVector<int> r;
                r.append(role);
                emit dataChanged(index(i), index(i), r);
            }
        }
    }
}
