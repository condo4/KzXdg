#include "kzxdg_plugin.h"

#include "xdgentry.h"
#include "xdgdesktopentries.h"
#include "xdgdesktopfilter.h"
#include "xdgautostart.h"
#include "xdgnotificationmanager.h"
#include "xdgnotificationactions.h"
#include "xdgstatusnotifieritem.h"
#include "xdgstatusnotifierhost.h"
#include "xdgstatusnotifierwatcher.h"

#include <qqml.h>

void KzXdgPlugin::registerTypes(const char *uri)
{
    // @uri org.kazoe.xdg
    qmlRegisterType<KaZoe::DesktopEntries>(uri, 1, 0, "DesktopEntries");
    qmlRegisterType<KaZoe::Entry>(uri, 1, 0, "Entry");
    qmlRegisterType<KaZoe::DesktopFilter>(uri, 1, 0, "DesktopFilter");
    qmlRegisterType<KaZoe::AutoStart>(uri, 1, 0, "AutoStart");
    qmlRegisterType<KaZoe::NotificationManager>(uri, 1, 0, "NotificationManager");
    qmlRegisterType<KaZoe::NotificationActions>(uri, 1, 0, "NotificationActions");
    qmlRegisterType<KaZoe::StatusNotifierItem>(uri, 1, 0, "StatusNotifierItem");
    qmlRegisterType<KaZoe::StatusNotifierHost>(uri, 1, 0, "StatusNotifierHost");
    qmlRegisterType<KaZoe::StatusNotifierWatcher>(uri, 1, 0, "StatusNotifierWatcher");
}
