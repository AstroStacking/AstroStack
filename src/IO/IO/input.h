#pragma once
#include <IO/config.h>
#include <IO/io.h>

#include <QtCore/QStringList>
#include <QtWidgets/QWidget>

namespace astro
{
/**
 Interface to read images
 */
class ASTRO_IO_EXPORT InputInterface
{
public:
    InputInterface();
    virtual ~InputInterface();
    /// List of supported extensions
    virtual QStringList filters() const = 0;
    /// open an image
    virtual ImageTypePtr open(QString filename, QWidget* parent) const = 0;

    /// Load an image based on the extension
    static AstroImage loadImg(QString path, QWidget* parent);
};

} // namespace astro

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(astro::InputInterface, "org.Astro.IO.InputInterface")
QT_END_NAMESPACE
