#include "output.h"

#include <IO/itkoutput.h>
#include <Plugin/pluginfactory.h>

#include <QtCore/QFileInfo>

namespace astro
{

namespace
{
std::vector<OutputInterface*> scanPlugins()
{
    std::vector<OutputInterface*> plugins;
    plugins.push_back(new ITKOutputPlugin());
    for (auto object : PluginFactory::get().getPluginsFor<OutputInterface*>())
    {
        auto* plugin = qobject_cast<OutputInterface*>(object);
        plugins.push_back(plugin);
    }
    return plugins;
}

const std::vector<OutputInterface*>& getPlugins()
{
    static std::vector<OutputInterface*> plugins = scanPlugins();
    return plugins;
}

} // namespace

OutputInterface::~OutputInterface() = default;

void OutputInterface::saveImg(ImageTypePtr img, QString path, QWidget* parent)
{
    QFileInfo info(path);
    QString extension = info.completeSuffix();

    const auto& plugins = getPlugins();
    for (auto plugin : plugins)
    {
        if (plugin->filters().count(extension))
        {
            plugin->save(img, path, parent);
            return;
        }
    }
}

} // namespace astro
