#ifndef XDGAUTOSTART_H
#define XDGAUTOSTART_H

#include <QObject>
#include "xdgentries.h"


namespace xdg {
    class AutoStart : public QObject
    {
        Q_OBJECT
    public:
        explicit AutoStart(QObject *parent = nullptr);
        xdg::Entries m_entries;

    public slots:
        void start(QString key = QString(), QString value = QString());

    signals:

    };
}

#endif // XDGAUTOSTART_H
