#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include "xdgdesktopentries.h"
#include "xdgentry.h"
#include "xdgentries.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    KaZoe::Entries entries;
    entries.addDirectory("/usr/share/applications");
/*
    QTimer t;
    QObject::connect(&t, &QTimer::timeout, [](){
        XdgDesktopEntries ent;
        for(int i = 0; i < ent.rowCount(); i++)
        {
            QVariant data = ent.data(ent.index(i), XdgDesktopEntries::DesktopFileRole);
            qDebug() << data.toString();
        }
        //QCoreApplication::exit(0);
    });

    t.setInterval(0);
    t.setSingleShot(true);
    t.start();
    */
    return a.exec();
}
