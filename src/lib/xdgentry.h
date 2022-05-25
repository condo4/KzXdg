#ifndef XDGENTRY_H
#define XDGENTRY_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QProcess>
#include <QSettings>

#define XDG_EXEC            "Desktop Entry/Exec"
#define XDG_ICON            "Desktop Entry/Icon"
#define XDG_NAME            "Desktop Entry/Name"
#define XDG_KEYWORDS        "Desktop Entry/Keywords"
#define XDG_NODISPLAY       "Desktop Entry/NoDisplay"
#define XDG_CATEGORIES      "Desktop Entry/Categories"

#define XDG_ACTION_START    "Desktop Action Start/Exec"
#define XDG_ACTION_STOP     "Desktop Action Stop/Exec"

namespace xdg {
    class Entry: public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString appId READ appId WRITE setAppId NOTIFY appIdChanged)
        Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
        Q_PROPERTY(QString desktopFile READ path WRITE setDesktopFile NOTIFY desktopFileChanged)

        QString m_path;
        QSettings *m_file {nullptr};
        QFileSystemWatcher m_watcher;
        QProcess m_process;
        mutable QMap<QString, QString> m_used;

    private slots:
        void _desktopFileChanged(const QString &path);
        void _processStateChanged(QProcess::ProcessState newState);

    public:
        explicit Entry(QString desktopfile, QObject *parent = nullptr);
        explicit Entry(QObject *parent = nullptr);
        explicit Entry(const xdg::Entry& origin);

        QString path() const;
        QString appId() const;
        QString icon() const;
        int pid() const;
        void setDesktopFile(QString desktopFile);
        void setAppId(QString appId);

    public slots:
        void start(QStringList args);
        void stop(QStringList args);
        QString data(const QString &key) const;

    signals:
        void dataChanged(const QString key, const QString value);
        void raiseProcess(int pid);
        void removed(const QString path);
        void appIdChanged(QString appId);
        void iconChanged(QString icon);
        void desktopFileChanged(QString desktopFile);
    };
}

Q_DECLARE_METATYPE(const xdg::Entry*)

#endif // XDGENTRY_H
