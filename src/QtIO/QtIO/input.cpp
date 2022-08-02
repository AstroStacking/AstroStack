#include "input.h"

#include <Plugin/pluginfactory.h>
#include <QtIO/itkinput.h>

#include <QtCore/QFileInfo>

namespace astro
{

namespace
{
std::vector<InputInterface*> scanPlugins()
{
    std::vector<InputInterface*> plugins;
    plugins.push_back(new ITKInputPlugin());
    for (auto object : PluginFactory::get().getPluginsFor<InputInterface*>())
    {
        auto* plugin = qobject_cast<InputInterface*>(object);
        plugins.push_back(plugin);
    }
    return plugins;
}

const std::vector<InputInterface*>& getPlugins()
{
    static std::vector<InputInterface*> plugins = scanPlugins();
    return plugins;
}

} // namespace

InputInterface::InputInterface() = default;
InputInterface::~InputInterface() = default;

AstroImage InputInterface::loadImg(QString path, QWidget* parent)
{
    QFileInfo info(path);
    QString extension = info.completeSuffix();

    const auto& plugins = getPlugins();
    for (auto plugin : plugins)
    {
        if (plugin->filters().count(extension))
        {
            ImageTypePtr img = plugin->open(path, parent);
            if (img)
            {
#if ASTRO_HAVE_EXIV2
                auto image = Exiv2::ImageFactory::open(path.toStdString());
                assert(image.get() != 0);
                image->readMetadata();
#endif
                return AstroImage(img
#if ASTRO_HAVE_EXIV2
                                  ,
                                  image->exifData()
#endif
                );
            }
        }
    }
    return {};
}

} // namespace astro
