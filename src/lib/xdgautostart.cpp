#include "xdgautostart.h"
#include "xdgbasedir.h"
#include "xdgentry.h"
#include "debug.h"

xdg::AutoStart::AutoStart(QObject *parent)
    : QObject{parent}
{
    qCDebug(AUTO_START) << "Create Auto start";
    QStringList configdirs = xdg::configDirs();
    for (auto dir = configdirs.rbegin(); dir != configdirs.rend(); ++dir)
    {
        m_entries.addDirectory(*dir + "/autostart");
    }
    m_entries.addDirectory(xdg::configHome() + "/autostart");
}

void xdg::AutoStart::start(QString key, QString value)
{
    qCDebug(AUTO_START) << "Start " << key << value;
    for(xdg::Entry *entry: m_entries.getEntries())
    {
        if((!value.isNull()) && (!key.isNull()))
        {
            QString c = entry->data(key);
            if(c.trimmed().toLower() == value.trimmed().toLower())
            {
                qCDebug(AUTO_START) << "Start app " << entry->appId() << "with" << key << value;
                entry->start(QStringList());
            }
        }
        else
        {
            qCDebug(AUTO_START) << "Start app " << entry->appId();
            entry->start(QStringList());
        }
    }
}
