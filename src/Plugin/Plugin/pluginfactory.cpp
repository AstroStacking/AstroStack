#include "pluginfactory.h"

#include <QtCore/QCoreApplication>

namespace astro
{
namespace
{
void scanStaticPlugins(PluginFactory::PluginContainer& container, const std::string& interface)
{
    const auto staticInstances = QPluginLoader::staticInstances();
    for (QObject* plugin : staticInstances)
    {
        auto* obj = plugin->qt_metacast(interface.c_str());
        if (obj != nullptr)
        {
            container.push_back(plugin);
        }
    }
    return container;
}

void scanDynamicPlugins(PluginFactory::PluginContainer& container, const std::string& interface)
{
    QDir pluginsDir = PluginFactory::getRootPath();
    pluginsDir.cd("plugins");

    const auto entryList = pluginsDir.entryList(QDir::Files);
    for (const QString& fileName : entryList)
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject* plugin = loader.instance();
        if (plugin != nullptr)
        {
            auto* obj = plugin->qt_metacast(interface.c_str());
            if (obj != nullptr)
            {
                container.push_back(plugin);
            }
        }
    }

    return container;
}
} // namespace

PluginFactory::PluginFactory() = default;

PluginFactory& PluginFactory::get()
{
    static PluginFactory facility;
    return facility;
}

QDir PluginFactory::getRootPath()
{
    QDir rootDir = QDir(QCoreApplication::applicationDirPath());

    if (rootDir.dirName().toLower() == "Debug" || rootDir.dirName().toLower() == "Release")
    {
        rootDir.cdUp();
    }
    if (rootDir.dirName().toLower() == "bin")
    {
        rootDir.cdUp();
    }

#if defined(Q_OS_MAC)
    if (rootDir.dirName() == "MacOS")
    {
        rootDir.cdUp();
        rootDir.cdUp();
        rootDir.cdUp();
    }
#endif
    return rootDir;
}

const PluginFactory::PluginContainer& PluginFactory::getPluginForInterface(const std::string& interface)
{
    auto it = m_cache.find(interface);
    if (it != m_cache.end())
    {
        return it->second;
    }
    auto& container = m_cache[interface];
    scanStaticPlugins(container, interface);
    scanDynamicPlugins(container, interface);
    return container;
}

} // namespace astro
