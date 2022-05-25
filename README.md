# XDG Free Desktop Qt implementation

This module is used to help the creation of project supporting Freedesktop Specifications

[Freedesktop Specifications can be find her](https://specifications.freedesktop.org/)

FreeDesktop (or XDG) is an extention of GNU and POSIX specifications.

This is why some "path" like library dirs (/lib:/usr/lib) is not managed by XDG, but by POSIX

The Specification hierarchy is:

POSIX > GNU > XDG

When POSIX or GNU concern the whole system, XDG is more restrictive for the "Desktop" (the Gui used by the USER)

KzXdgUtils provide a library using Qt Core only, and a Qt Module to use it directly in QML.

# Build and deploy

```sh
git clone https://kazoe.org/cgit/kazoe/KzXdg.git
cd KzXdg
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make
sudo make install
```


# XdgUtils Specification and API

## basedir-spec

[basedir-spec](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html) is implemented by the library.

The order of base directories denotes their importance; the first directory listed is the most important. When the same information is defined in multiple places the information defined relative to the more important base directory takes precedent. The base directory defined by $XDG_DATA_HOME is considered more important than any of the base directories defined by $XDG_DATA_DIRS. The base directory defined by $XDG_CONFIG_HOME is considered more important than any of the base directories defined by $XDG_CONFIG_DIRS.

This function are available by including xdgbasedir.h


```c
#include <xdgbasedir.h>
```

All XDG base directory can be set by environment variables.

$XDG_DATA_HOME want to say, XDG_DATA_HOME environement variable.

### QString xdg::dataHome()
XDG_DATA_HOME defines the base directory relative to which user-specific data files should be stored.
Return $XDG_DATA_HOME content or $HOME/.local/share if not set

### QString xdg::configHome()
XDG_CONFIG_HOME defines the base directory relative to which user-specific configuration files should be stored.
Return $XDG_CONFIG_HOME content or $HOME/.config if not set

### QString xdg::stateHome()
XDG_STATE_HOME defines the base directory relative to which user-specific state files should be stored.
If $XDG_STATE_HOME is either not set or empty, a default equal to $HOME/.local/state should be used.

The $XDG_STATE_HOME contains state data that should persist between (application) restarts, but that is not important or portable enough to the user that it should be stored in $XDG_DATA_HOME. It may contain:

- actions history (logs, history, recently used files, ...)
- current state of the application that can be reused on a restart (view, layout, open files, undo history, …)

### QString xdg::dataDirs()
$XDG_DATA_DIRS defines the preference-ordered set of base directories to search for data files in addition to the $XDG_DATA_HOME base directory.
The directories in $XDG_DATA_DIRS should be seperated with a colon ':'.

If $XDG_DATA_DIRS is either not set or empty, a value equal to /usr/local/share/:/usr/share/ should be used.

### QString xdg::configDirs()
XDG_CONFIG_DIRS defines the preference-ordered set of base directories to search for configuration files in addition to the $XDG_CONFIG_HOME base directory. The directories in $XDG_CONFIG_DIRS should be seperated with a colon ':'.

If $XDG_CONFIG_DIRS is either not set or empty, a value equal to /etc/xdg should be used.

### QString xdg::cacheHome()
XDG_CACHE_HOME defines the base directory relative to which user-specific non-essential data files should be stored. If $XDG_CACHE_HOME is either not set or empty, a default equal to $HOME/.cache should be used.

### QString xdg::runtimeDir()
XDG_RUNTIME_DIR defines the base directory relative to which user-specific non-essential runtime files and other file objects (such as sockets, named pipes, ...) should be stored. The directory MUST be owned by the user, and he MUST be the only one having read and write access to it. Its Unix access mode MUST be 0700.

The lifetime of the directory MUST be bound to the user being logged in. It MUST be created when the user first logs in and if the user fully logs out the directory MUST be removed. If the user logs in more than once he should get pointed to the same directory, and it is mandatory that the directory continues to exist from his first login to his last logout on the system, and not removed in between. Files in the directory MUST not survive reboot or a full logout/login cycle.

The directory MUST be on a local file system and not shared with any other system. The directory MUST by fully-featured by the standards of the operating system. More specifically, on Unix-like operating systems AF_UNIX sockets, symbolic links, hard links, proper permissions, file locking, sparse files, memory mapping, file change notifications, a reliable hard link count must be supported, and no restrictions on the file name character set should be imposed. Files in this directory MAY be subjected to periodic clean-up. To ensure that your files are not removed, they should have their access time timestamp modified at least once every 6 hours of monotonic time or the 'sticky' bit should be set on the file.

If $XDG_RUNTIME_DIR is not set applications should fall back to a replacement directory with similar capabilities and print a warning message. Applications should use this directory for communication and synchronization purposes and should not place larger files in it, since it might reside in runtime memory and cannot necessarily be swapped out to disk.


# Limitation

Because of QSettings limitation, categories filed support only 1 category. This is because in INI format ";" is the begin of a comment
So, value of "Categories=Qt;KDE;Settings;" will return only "Qt"




# XdgNotificationServer

To use XdgNotificationServer, you have to:
- Inherit your server class from XdgNotificationServer (#include <xdgnotificationserver.h>) ex: MyNotificationServer
- Instanciate your server class ex: MyNotificationServer myserver.
- create the adaptor ex: new NotificationsAdaptor(&myserver)
- Register to dbus session: XdgNotificationServer::RegisterServerToDbus(myserver)


# Debuging

Debug traces in this library use QLoggingCategory. Many way exists to display additional debug traces (see https://doc.qt.io/qt-6/qloggingcategory.html )
For example, you can create /etc/xdg/QtProject/qtlogging.ini

