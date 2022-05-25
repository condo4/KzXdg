#include "xdgdesktopfilter.h"
#include "xdgdesktopentries.h"
#include "xdgentry.h"
#include <qdebug.h>

KaZoe::DesktopFilter::DesktopFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

QObject *KaZoe::DesktopFilter::model() const
{
    return static_cast<QObject*>(m_model);
}

QString KaZoe::DesktopFilter::category() const
{
    return m_category;
}

QString KaZoe::DesktopFilter::section() const
{
    return m_section;
}


bool KaZoe::DesktopFilter::showAll() const
{
    return m_showAll;
}

bool KaZoe::DesktopFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QVariant ventry = sourceModel()->data(index, KaZoe::DesktopEntries::EntryRole);
    const KaZoe::Entry *entry = qvariant_cast<const KaZoe::Entry *>(ventry);
    if(!entry)
    {
        qWarning() << "Can't retrive entry for " << sourceModel()->data(index, KaZoe::DesktopEntries::NameRole).toString();
        return false;
    }

    if(!m_section.isEmpty() && entry->data(m_section + "/Exec").isEmpty())
    {
        return false;
    }
    if(entry->data(XDG_NAME).isEmpty())
        return false;
    if(!m_showAll && entry->data(XDG_NODISPLAY).toLower() == "true")
        return false;
    if(m_category.size() > 0 && entry->data(XDG_CATEGORIES) != m_category)
        return false;

    return true;
}


QVariant KaZoe::DesktopFilter::data(const QModelIndex &index, int role) const
{
    if(!m_section.isEmpty())
    {
        const KaZoe::Entry *entry = QSortFilterProxyModel::data(index, KaZoe::DesktopEntries::EntryRole).value<const KaZoe::Entry *>();
        if(entry)
        {
            QString res;
            switch(role) {
                case Qt::DisplayRole:
                case KaZoe::DesktopEntries::NameRole:
                    res = entry->data(m_section + "/Name");
                    return (res.isEmpty())?(res):(entry->data(XDG_NAME));
                case KaZoe::DesktopEntries::ExecRole:
                    res = entry->data(m_section + "/Exec");
                    return (!res.isEmpty())?(res):(entry->data(XDG_EXEC));
                case KaZoe::DesktopEntries::IconRole:
                    res = entry->data(m_section + "/Icon");
                    return (!res.isEmpty())?(res):(entry->data(XDG_ICON));
                case KaZoe::DesktopEntries::CategoryRole:
                    res = entry->data(m_section + "/Categories");
                    return (!res.isEmpty())?(res):(entry->data(XDG_CATEGORIES));
                case KaZoe::DesktopEntries::KeywordsRole:
                    res = entry->data(m_section + "/Keywords");
                    return (!res.isEmpty())?(res):(entry->data(XDG_KEYWORDS));
            }
        }
    }
    return QSortFilterProxyModel::data(index, role);
}

void KaZoe::DesktopFilter::setModel(QObject *model)
{
    if (m_model == model)
        return;

    KaZoe::DesktopEntries *entries = qobject_cast<KaZoe::DesktopEntries*>(model);
    if(entries)
    {
        m_model = entries;
        setSourceModel(m_model);
        emit modelChanged(m_model);
    }
}

void KaZoe::DesktopFilter::setCategory(QString category)
{
    if (m_category == category)
        return;

    m_category = category;
    invalidateFilter();
    emit categoryChanged(m_category);
}

void KaZoe::DesktopFilter::setSection(QString section)
{
    if (m_section == section)
        return;

    m_section = section;
    invalidateFilter();
    emit sectionChanged(m_section);
}

void KaZoe::DesktopFilter::setShowAll(bool showall)
{
    if (m_showAll == showall)
        return;

    m_showAll = showall;
    invalidateFilter();
    emit showAllChanged(m_showAll);
}
