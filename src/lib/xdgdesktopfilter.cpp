#include "xdgdesktopfilter.h"
#include "xdgdesktopentries.h"
#include "xdgentry.h"
#include <qdebug.h>

xdg::DesktopFilter::DesktopFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

QObject *xdg::DesktopFilter::model() const
{
    return static_cast<QObject*>(m_model);
}

QString xdg::DesktopFilter::category() const
{
    return m_category;
}

QString xdg::DesktopFilter::section() const
{
    return m_section;
}


bool xdg::DesktopFilter::showAll() const
{
    return m_showAll;
}

bool xdg::DesktopFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QVariant ventry = sourceModel()->data(index, xdg::DesktopEntries::EntryRole);
    const xdg::Entry *entry = qvariant_cast<const xdg::Entry *>(ventry);
    if(!entry)
    {
        qWarning() << "Can't retrive entry for " << sourceModel()->data(index, xdg::DesktopEntries::NameRole).toString();
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


QVariant xdg::DesktopFilter::data(const QModelIndex &index, int role) const
{
    if(!m_section.isEmpty())
    {
        const xdg::Entry *entry = QSortFilterProxyModel::data(index, xdg::DesktopEntries::EntryRole).value<const xdg::Entry *>();
        if(entry)
        {
            QString res;
            switch(role) {
                case Qt::DisplayRole:
                case xdg::DesktopEntries::NameRole:
                    res = entry->data(m_section + "/Name");
                    return (res.isEmpty())?(res):(entry->data(XDG_NAME));
                case xdg::DesktopEntries::ExecRole:
                    res = entry->data(m_section + "/Exec");
                    return (!res.isEmpty())?(res):(entry->data(XDG_EXEC));
                case xdg::DesktopEntries::IconRole:
                    res = entry->data(m_section + "/Icon");
                    return (!res.isEmpty())?(res):(entry->data(XDG_ICON));
                case xdg::DesktopEntries::CategoryRole:
                    res = entry->data(m_section + "/Categories");
                    return (!res.isEmpty())?(res):(entry->data(XDG_CATEGORIES));
                case xdg::DesktopEntries::KeywordsRole:
                    res = entry->data(m_section + "/Keywords");
                    return (!res.isEmpty())?(res):(entry->data(XDG_KEYWORDS));
            }
        }
    }
    return QSortFilterProxyModel::data(index, role);
}

void xdg::DesktopFilter::setModel(QObject *model)
{
    if (m_model == model)
        return;

    xdg::DesktopEntries *entries = qobject_cast<xdg::DesktopEntries*>(model);
    if(entries)
    {
        m_model = entries;
        setSourceModel(m_model);
        emit modelChanged(m_model);
    }
}

void xdg::DesktopFilter::setCategory(QString category)
{
    if (m_category == category)
        return;

    m_category = category;
    invalidateFilter();
    emit categoryChanged(m_category);
}

void xdg::DesktopFilter::setSection(QString section)
{
    if (m_section == section)
        return;

    m_section = section;
    invalidateFilter();
    emit sectionChanged(m_section);
}

void xdg::DesktopFilter::setShowAll(bool showall)
{
    if (m_showAll == showall)
        return;

    m_showAll = showall;
    invalidateFilter();
    emit showAllChanged(m_showAll);
}
