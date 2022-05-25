#include "xdgbasedir.h"
#include <QStringList>

static inline QString user()
{
    QString user = QString::fromUtf8(qgetenv("USER"));
    if(user.isEmpty()) return "root";
    return user;
}

static inline QString home()
{
    QString home = QString::fromUtf8(qgetenv("HOME"));
    if(home.isEmpty())
    {
        QString u = user();
        if(u == "root")
            return "/root";
        else
            return "/home/" + u;
    }
    return home;
}

static inline QString envOrDefault(const char *name, const QString &def)
{
    QString env = QString::fromUtf8(qgetenv(name));
    if(env.isEmpty())
    {
        return def;
    }
    return env;
}

QString xdg::dataHome()
{
    return envOrDefault("XDG_DATA_HOME", home() + "/.local/share");
}

QString xdg::configHome()
{
    return envOrDefault("XDG_CONFIG_HOME", home() + "/.config");
}

QString xdg::cacheHome()
{
    return envOrDefault("XDG_CACHE_HOME", home() + "/.cache");
}

QString xdg::stateHome()
{
    return envOrDefault("XDG_STATE_HOME", home() + "/.local/state");
}


QStringList xdg::dataDirs()
{
    QString dataDirs = QString::fromUtf8(qgetenv("XDG_DATA_DIRS"));
    if(dataDirs.isEmpty())
    {
        return QStringList() << "/usr/local/share" << "/usr/share";
    }
    return dataDirs.split(":");
}

QStringList xdg::configDirs()
{
    QString configDirs = QString::fromUtf8(qgetenv("XDG_CONFIG_DIRS"));
    if(configDirs.isEmpty())
    {
        return QStringList() << "/etc/xdg";
    }
    return configDirs.split(":");
}

QString xdg::runtimeDir()
{
    return envOrDefault("XDG_RUNTIME_DIR", QString("/run/user/%i").arg(user()));
}
