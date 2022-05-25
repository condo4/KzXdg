#include "kzxdgutils_plugin.h"

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

void KzXdgUtilsPlugin::registerTypes(const char *uri)
{
    // @uri xdgutils
    qmlRegisterType<xdg::DesktopEntries>(uri, 1, 0, "DesktopEntries");
    qmlRegisterType<xdg::Entry>(uri, 1, 0, "XdgEntry");
    qmlRegisterType<xdg::DesktopFilter>(uri, 1, 0, "DesktopFilter");
    qmlRegisterType<xdg::AutoStart>(uri, 1, 0, "AutoStart");
    qmlRegisterType<xdg::NotificationManager>(uri, 1, 0, "NotificationManager");
    qmlRegisterType<xdg::NotificationActions>(uri, 1, 0, "NotificationActions");
    qmlRegisterType<xdg::StatusNotifierItem>(uri, 1, 0, "StatusNotifierItem");
    qmlRegisterType<xdg::StatusNotifierHost>(uri, 1, 0, "StatusNotifierHost");
    qmlRegisterType<xdg::StatusNotifierWatcher>(uri, 1, 0, "StatusNotifierWatcher");
}
