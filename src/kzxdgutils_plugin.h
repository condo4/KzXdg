#ifndef KZXDGUTILS_PLUGIN_H
#define KZXDGUTILS_PLUGIN_H

#include <QQmlExtensionPlugin>

class KzXdgUtilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

#endif // KZXDGUTILS_PLUGIN_H
