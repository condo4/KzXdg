cmake_minimum_required(VERSION 3.14)

project(KzXdg VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED QML_MODULE_INSTALL_PATH)
    if(DEFINED OE_QMAKE_PATH_QML)
        set(QML_MODULE_INSTALL_PATH ${OE_QMAKE_PATH_QML} )
    else()
        set(QML_MODULE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/qt6/qml )
    endif()
endif()

find_package(Qt6 COMPONENTS Core Quick Qml DBus REQUIRED)

set(PLUGIN_SOURCES
    src/kzxdg_plugin.h src/kzxdg_plugin.cpp
    qml/qmldir
)

include_directories(src/lib)
set(LIBRARY_SOURCES
    src/lib/debug.cpp
    src/lib/xdgbasedir.cpp
    src/lib/xdgentry.cpp
    src/lib/xdgentries.cpp
    src/lib/xdgdesktopentries.cpp
    src/lib/xdgdesktopfilter.cpp
    src/lib/xdgautostart.cpp
    src/lib/xdgnotificationserver.cpp
    src/lib/xdgnotificationmessage.cpp
    src/lib/xdgnotificationmanager.cpp
    src/lib/xdgnotificationactions.cpp
    src/lib/xdgnotifier.cpp
    src/lib/xdgstatusnotifieritem.cpp
    src/lib/xdgstatusnotifierhost.cpp
    src/lib/xdgstatusnotifierwatcher.cpp
    cmake/CPack.cmake
)

set(LIBRARY_HEADERS_PRIVATE
    src/lib/debug.h
)

set(LIBRARY_HEADERS
    src/lib/xdgbasedir.h
    src/lib/xdgentry.h
    src/lib/xdgentries.h
    src/lib/xdgdesktopentries.h
    src/lib/xdgdesktopfilter.h
    src/lib/xdgautostart.h
    src/lib/xdgnotificationserver.h
    src/lib/xdgnotificationmessage.h
    src/lib/xdgnotificationmanager.h
    src/lib/xdgnotificationactions.h
    src/lib/xdgnotifier.h
    src/lib/xdgstatusnotifieritem.h
    src/lib/xdgstatusnotifierhost.h
    src/lib/xdgstatusnotifierwatcher.h
)


list(APPEND             DBUS_IFACE_SRCS api/org.freedesktop.Notifications.xml)
qt_add_dbus_adaptor(    DBUS_IFACE_SRCS api/org.freedesktop.Notifications.xml xdgnotificationserver.h XdgNotificationServer)
qt_add_dbus_interface(  DBUS_IFACE_SRCS api/org.freedesktop.Notifications.xml NotificationsIface)

list(APPEND             DBUS_IFACE_SRCS api/org.freedesktop.StatusNotifierItem.xml)
qt_add_dbus_adaptor(    DBUS_IFACE_SRCS api/org.freedesktop.StatusNotifierItem.xml xdgstatusnotifieritem.h KaZoe::StatusNotifierItem)
qt_add_dbus_interface(  DBUS_IFACE_SRCS api/org.freedesktop.StatusNotifierItem.xml XdgStatusNotifierItemIface)

list(APPEND             DBUS_IFACE_SRCS api/org.freedesktop.StatusNotifierWatcher.xml)
qt_add_dbus_adaptor(    DBUS_IFACE_SRCS api/org.freedesktop.StatusNotifierWatcher.xml xdgstatusnotifierwatcher.h KaZoe::StatusNotifierWatcher)
qt_add_dbus_interface(  DBUS_IFACE_SRCS api/org.freedesktop.StatusNotifierWatcher.xml XdgStatusNotifierWatcherIface)

add_library(KzXdgPlugin  SHARED ${PLUGIN_SOURCES})
add_library(KzXdg        SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS} ${LIBRARY_HEADERS_PRIVATE} ${DBUS_IFACE_SRCS} cmake/Config.cmake.in)
set_target_properties(KzXdg PROPERTIES PUBLIC_HEADER "${LIBRARY_HEADERS}")
set_target_properties(KzXdg PROPERTIES VERSION ${CMAKE_PROJECT_VERSION} SOVERSION 1)

target_compile_definitions(KzXdgPlugin
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(KzXdgPlugin
    PRIVATE
        Qt6::Qml
        KzXdg
)

target_link_libraries(KzXdg
    PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::DBus
)

add_executable(test_xdg src/test/test_xdg.cpp)
target_link_libraries(test_xdg PRIVATE KzXdg Qt6::Core)

install(TARGETS KzXdgPlugin DESTINATION ${QML_MODULE_INSTALL_PATH}/org/kazoe/xdg)
install(FILES qml/qmldir DESTINATION ${QML_MODULE_INSTALL_PATH}/org/kazoe/xdg)
install(TARGETS KzXdg PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/KaZoe/Xdg/")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/notificationsadaptor.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/KaZoe/Xdg" RENAME xdgnotificationsadaptor.h)

# CMake Module
include(CMakePackageConfigHelpers)
set(CMakeModuleName "KzXdg")
configure_package_config_file(
    cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${CMakeModuleName}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMakeModuleName}
)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${CMakeModuleName}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMakeModuleName}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${CMakeModuleName}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMakeModuleName}
)

include(cmake/CPack.cmake)
