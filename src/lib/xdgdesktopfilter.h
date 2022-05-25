#ifndef XDGDESKTOPFILTER_H
#define XDGDESKTOPFILTER_H

#include <QSortFilterProxyModel>
#include <QObject>
#include "xdgdesktopentries.h"

namespace KaZoe {
    class DesktopEntries;

    class DesktopFilter : public QSortFilterProxyModel
    {
        Q_OBJECT
        Q_PROPERTY(QObject *model READ model WRITE setModel NOTIFY modelChanged)
        Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)
        Q_PROPERTY(QString section READ section WRITE setSection NOTIFY sectionChanged)
        Q_PROPERTY(bool showAll READ showAll WRITE setShowAll NOTIFY showAllChanged)

        KaZoe::DesktopEntries *m_model;
        QString m_category;
        QString m_section;
        bool m_showAll {false};

    public:
        explicit DesktopFilter(QObject *parent = nullptr);
        QObject *model() const;
        QString category() const;
        QString section() const;
        bool showAll() const;
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    public slots:
        void setModel(QObject * model);
        void setCategory(QString category);
        void setSection(QString category);
        void setShowAll(bool showall);

    signals:
        void raise(int pid);
        void modelChanged(QObject * model);
        void categoryChanged(QString category);
        void sectionChanged(QString category);
        void showAllChanged(bool showall);
    };

}
#endif // XDGDESKTOPFILTER_H
