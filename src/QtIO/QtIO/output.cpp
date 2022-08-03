#include "output.h"

#include <Plugin/pluginfactory.h>
#include <QtIO/itkoutput.h>

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

OutputInterface::OutputInterface() = default;
OutputInterface::~OutputInterface() = default;

void OutputInterface::saveImg(const AstroImage& img, QString path, QWidget* parent)
{
    QFileInfo info(path);
    QString extension = info.completeSuffix();

    const auto& plugins = getPlugins();
    for (auto plugin : plugins)
    {
        if (plugin->filters().count(extension))
        {
            plugin->save(img.getImg(), path, parent);
            saveEnrichedImage(path.toStdString(), img);
            return;
        }
    }
}

} // namespace astro
