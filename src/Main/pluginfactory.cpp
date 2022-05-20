#include "pluginfactory.h"

namespace astro
{
PluginFactory::PluginFactory() = default;

PluginFactory& PluginFactory::get()
{
    static PluginFactory facility;
    return facility;
}

const PluginFactory::PluginContainer& PluginFactory::getPluginForInterface(const std::string& interface)
{
    auto it = m_cache.find(interface);
    if (it != m_cache.end())
    {
        return it->second;
    }
    auto& container = m_cache[interface];
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

} // namespace astro
