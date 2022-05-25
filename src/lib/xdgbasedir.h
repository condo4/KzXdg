/*! \file xdgbasedir.h
    \brief Implementation of basedir-spec
    \author Fabien Proriol

    Implementation of https://specifications.freedesktop.org/basedir-spec/basedir-spec-0.6.html
*/

#ifndef XDGBASEDIR_H
#define XDGBASEDIR_H

#include <QString>

//! XDG basedir implementation
namespace xdg {
/*!
 *  \addtogroup XDG
 *  @{
 */

/*! \fn QString dataHome()
    \brief Return XDG_DATA_HOME
            XDG_DATA_HOME defines the base directory relative to
            which user-specific data files should be stored.
    \return $XDG_DATA_HOME content or $HOME/.local/share if not set
*/
    QString dataHome();


/*! \fn QString configHome()
    \brief Return XDG_CONFIG_HOME
            XDG_CONFIG_HOME defines the base directory relative to
            which user-specific configuration files should be stored.
    \return $XDG_CONFIG_HOME content or $HOME/.config if not set
*/
    QString configHome();


/*! \fn QString stateHome()
    \brief Return XDG_STATE_HOME
            XDG_STATE_HOME defines the base directory relative to
            which user-specific state files should be stored.
    \return $XDG_STATE_HOME content or $HOME/.local/state if not set
*/
    QString stateHome();


/*! \fn QStringList dataDirs()
    \brief Return XDG_DATA_DIRS
            XDG_DATA_DIRS defines the preference-ordered set of
            base directories to search for data files in addition
            to the $XDG_DATA_HOME base directory.
            The directories in $XDG_DATA_DIRS should be seperated
            with a colon ':'.
    \return $XDG_DATA_DIRS content or /usr/local/share:/usr/share if not set
*/
    QStringList dataDirs();

/*! \fn QStringList dataDirs()
    \brief Return XDG_CONFIG_DIRS
            XDG_CONFIG_DIRS defines the preference-ordered set of
            base directories to search for configuration files in addition
            to the $XDG_CONFIG_HOME base directory.
            The directories in $XDG_CONFIG_DIRS should be seperated
            with a colon ':'.
    \return $XDG_CONFIG_DIRS content or /etc/xdg if not set
*/
    QStringList configDirs();


/*! \fn QString cacheHome()
    \brief Return XDG_CONFIG_DIRS
            XDG_CACHE_HOME defines the base directory relative to
            which user-specific non-essential data files should be
            stored.
            If $XDG_CACHE_HOME is either not set or empty, a default
            equal to $HOME/.cache should be used.
    \return $XDG_CONFIG_DIRS content or $HOME/.cache if not set
*/
    QString cacheHome();


/*! \fn QString runtimeDir()
    \brief Return XDG_CONFIG_DIRS
            XDG_RUNTIME_DIR defines the base directory relative to
            which user-specific non-essential runtime files and other
            file objects (such as sockets, named pipes, ...) should
            be stored. The directory MUST be owned by the user,
            and he MUST be the only one having read and write access
            to it. Its Unix access mode MUST be 0700.
    \return $XDG_RUNTIME_DIR content or emit a warning and return /run/user/$USER
*/
    QString runtimeDir();

/*! @} End of XDG*/
}

#endif // XDGBASEDIR_H
