#include "exponential.h"

#include <Plugin/pluginfactory.h>

namespace astro
{

Exponential::~Exponential() = default;

QString Exponential::name() const
{
    return "Exponential";
}

QString Exponential::explanation() const
{
    return tr("Raises the image values to the power of a parameter");
}

Exponential::GUI* Exponential::generateGUI(QWidget* parent) const
{
    return nullptr;
}

Exponential::GUI::~GUI() = default;

ImageTypePtr Exponential::GUI::process(ImageTypePtr img, QPromise<void>& promise)
{
    return img;
}

} // namespace astro
