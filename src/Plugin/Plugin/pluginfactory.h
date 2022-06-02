#include <QtCore/QDir>
#include <QtCore/QPluginLoader>

#include <string>
#include <unordered_map>
#include <vector>

namespace astro
{
class PluginFactory
{
    PluginFactory();

public:
    static PluginFactory& get();

    using PluginContainer = std::vector<QObject*>;

    template<typename T>
    const PluginContainer& getPluginsFor()
    {
        return getPluginForInterface(qobject_interface_iid<T>());
    }

    static QDir getRootPath();

protected:
    const PluginContainer& getPluginForInterface(const std::string& interface);
    std::unordered_map<std::string, PluginContainer> m_cache;
};
} // namespace astro
