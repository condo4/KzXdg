#include "xdgentry.h"
#include <QSettings>
#include <qdebug.h>
#include <signal.h>
#include <vector>
#include <string>
#include <QFileInfo>
#include "xdgbasedir.h"

static inline QStringList tokenize(const QString& command)
{
    std::string s = command.toStdString();
    std::vector<std::string> result;

    std::string token;
    char quote{};
    bool escape{false};

    for (char c : s)
    {
        if (escape)
        {
            escape = false;
            if (quote && c != '\\' && c != quote)
                token += '\\';
            token += c;
        }
        else if (c == '\\')
        {
            escape = true;
        }
        else if (!quote && (c == '\'' || c == '\"'))
        {
            quote = c;
        }
        else if (quote && c == quote)
        {
            quote = '\0';
            if (token.empty())
                result.emplace_back();
        }
        else if (!isspace(c) || quote)
        {
            token += c;
        }
        else if (!token.empty())
        {
            result.push_back(std::move(token));
            token.clear();
        }
    }

    if (!token.empty())
    {
        result.push_back(std::move(token));
        token.clear();
    }


    QStringList l;
    for (auto t: result)
        l.append(QString(t.c_str()));
    return l;
}

static inline QStringList args_substitution(const QStringList &tokens, const QStringList &args)
{
    QStringList out;
    for (auto t : tokens)
    {
        if (t == "%d" || t == "%D" || t == "%n" || t == "%N" || t == "%v" || t == "%m") { //deprectaed stuff
            continue;
        } else if (t == "%i" || t == "%c") { //unsupported stuff
            qWarning() << "XdgEntry: %i and %c are not supported (found in " << tokens.join(" ")<<")";
            continue;
        } else if (t == "%f" || t == "%F") { // files
            if (t == "%f")
            {
                if (args.length() > 0)
                {
                    out.append(args[0]);
                }
                else
                {
                    out.append(args);
                }
            }
        } else if (t == "%u" || t == "%U") { // urls
            // TODO : handle URLs properly
            if (t == "%u")
            {
                if (args.length() > 0)
                {
                    out.append(args[0]);
                }
                else
                {
                    out.append(args);
                }
            }
        } else {
            out.append(t);
        }
    }
    return out;
}


xdg::Entry::Entry(QString desktopfile, QObject *parent)
    : QObject(parent)
    , m_path(desktopfile)
    , m_file(new QSettings(desktopfile, QSettings::IniFormat, this))
    , m_watcher(QStringList() << desktopfile)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("QT_QPA_PLATFORM", "wayland-egl");
    env.remove("QT_IM_MODULE");
    m_process.setProcessEnvironment(env);
    QObject::connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &Entry::_desktopFileChanged, Qt::QueuedConnection);
    QObject::connect(&m_process, &QProcess::stateChanged, this, &Entry::_processStateChanged);
}

xdg::Entry::Entry(QObject *parent)
    : QObject(parent)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("QT_QPA_PLATFORM", "wayland-egl");
    env.remove("QT_IM_MODULE");
    m_process.setProcessEnvironment(env);
    QObject::connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &Entry::_desktopFileChanged, Qt::QueuedConnection);
    QObject::connect(&m_process, &QProcess::stateChanged, this, &Entry::_processStateChanged);
}

xdg::Entry::Entry(const Entry &origin)
    : QObject(origin.parent())
    , m_path(origin.path())
    , m_file(new QSettings(origin.path(), QSettings::IniFormat, this))
    , m_watcher(QStringList() << origin.path())
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("QT_QPA_PLATFORM", "wayland-egl");
    env.remove("QT_IM_MODULE");
    m_process.setProcessEnvironment(env);
    QObject::connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &Entry::_desktopFileChanged, Qt::QueuedConnection);
    QObject::connect(&m_process, &QProcess::stateChanged, this, &Entry::_processStateChanged);
}


QString xdg::Entry::data(const QString &key) const
{
    if(!m_file)
        return QString();
    if(key == XDG_KEYWORDS)
    {
        QString keywords = m_file->value(key).toString();
        if(keywords.contains("RemoteProcess"))
            return keywords;

        switch(m_process.state())
        {
        case QProcess::Running:
            keywords.append(" running");
            break;
        case QProcess::NotRunning:
            keywords.append(" stopped");
            break;
        case QProcess::Starting:
            keywords.append(" starting");
            break;
        }
        return keywords;
    }
    if(!m_used.contains(key))
        m_used[key] = m_file->value(key).toString();
    return m_file->value(key).toString();
}

void xdg::Entry::start(QStringList args)
{
    QString exec = data(XDG_EXEC);
    QString startExec = data(XDG_ACTION_START);

    if(exec.size() > 0)
    {
        QStringList param = tokenize(exec);
        QString cmd = param.takeFirst();
        QStringList argv = args_substitution(param, args);

        if(m_process.state() == QProcess::Running)
        {
            emit raiseProcess(m_process.processId());
        }
        else
        {
            m_process.start(cmd, argv);
        }
    }
    else if(startExec.size() > 0)
    {
        QStringList param = tokenize(startExec);
        QString cmd = param.takeFirst();
        QStringList argv = args_substitution(param, args);

        m_process.start(cmd, argv);
    }
    else
    {
        qWarning() << "Can't find a way to start " << m_path;
    }
}

void xdg::Entry::stop(QStringList args)
{
    QString stopExec = data(XDG_ACTION_STOP);

    if(stopExec.size() > 0)
    {
        QStringList param = tokenize(stopExec);
        QString cmd = param.takeFirst();
        QStringList argv = args_substitution(param, args);

        m_process.start(cmd, argv);
    }
    else
    {
        if(m_process.state() == QProcess::NotRunning)
            return;
        kill(m_process.processId(), SIGTERM);
    }
}

QString xdg::Entry::path() const
{
    return m_path;
}

QString xdg::Entry::appId() const
{
    QFileInfo fi(path());
    return fi.completeBaseName();
}

QString xdg::Entry::icon() const
{
    return data(XDG_ICON);
}

int xdg::Entry::pid() const
{
    return m_process.processId();
}

void xdg::Entry::setDesktopFile(QString desktopFile)
{
    m_path = desktopFile;
    m_file = new QSettings(desktopFile, QSettings::IniFormat);
    m_watcher.addPaths(QStringList() << desktopFile);
    emit iconChanged(data(XDG_ICON));
}

void xdg::Entry::setAppId(QString appId)
{
    /* Search for appid destopfile */
    QString filename;

    filename = xdg::dataHome() + "/applications/" + appId + ".desktop";
    if(filename.length())
    {
        QFileInfo info(filename);
        if(info.exists())
        {
            setDesktopFile(filename);
            return;
        }
    }

    for(QString dir: xdg::dataDirs())
    {
        filename = dir + "/applications/" + appId + ".desktop";
        QFileInfo info(filename);
        if(info.exists())
        {
            setDesktopFile(filename);
            return;
        }
    }
    qWarning() << "Can't find Desktop Entry for " + appId;
}


void xdg::Entry::_desktopFileChanged(const QString &path)
{
    if(!m_file)
        return;
    QFileInfo info(path);
    if(!info.isFile())
    {
        emit removed(path);
        return;
    }
    m_file->sync();
    for(const QString &key: m_used.keys())
    {
        if(m_file->value(key).toString() != m_used[key])
        {
            m_used[key] = m_file->value(key).toString();
            emit dataChanged(key, m_used[key]);
        }
    }
}

void xdg::Entry::_processStateChanged(QProcess::ProcessState newState)
{
    emit dataChanged(XDG_KEYWORDS, data(XDG_KEYWORDS));
}
