#include "input.h"

#include <pluginfactory.h>

#include <QtCore/QFileInfo>

namespace astro
{

InputInterface::~InputInterface() = default;

ImageTypePtr InputInterface::loadImg(QString path, QWidget* parent)
{
    QFileInfo info(path);
    QString extension = info.completeSuffix();

    const auto& plugins = PluginFactory::get().getPluginsFor<InputInterface*>();
    for (auto object : plugins)
    {
        auto* plugin = qobject_cast<InputInterface*>(object);
        if (plugin->filters().count(extension))
        {
            ImageTypePtr img = plugin->open(path, parent);
            if (img)
            {
                return img;
            }
        }
    }
    return {};
}

} // namespace astro
