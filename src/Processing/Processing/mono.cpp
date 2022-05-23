#include "mono.h"

#include <Plugin/pluginfactory.h>

namespace astro
{

namespace
{
std::vector<MonoInterface*> scanPlugins()
{
    std::vector<MonoInterface*> plugins;
    //plugins.push_back(new ITKInputPlugin());
    for (auto object : PluginFactory::get().getPluginsFor<MonoInterface*>())
    {
        auto* plugin = qobject_cast<MonoInterface*>(object);
        plugins.push_back(plugin);
    }
    return plugins;
}
} // namespace

MonoInterface::~MonoInterface() = default;

MonoInterface::GUI::~GUI() = default;

const std::vector<MonoInterface*>& MonoInterface::getPlugins()
{
    static std::vector<MonoInterface*> plugins = scanPlugins();
    return plugins;
}

} // namespace astro
