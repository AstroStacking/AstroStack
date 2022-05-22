#pragma once
#include <IO/io.h>

#include <QtCore/QStringList>
#include <QtPlugin>
#include <QtWidgets/QWidget>

namespace astro
{

class InputInterface
{
public:
    virtual ~InputInterface();

    virtual QStringList filters() const = 0;
    virtual ImageTypePtr open(QString filename, QWidget* parent) const = 0;

    static ImageTypePtr loadImg(QString path, QWidget* parent);
};

} // namespace astro

QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(astro::InputInterface, "org.Astro.IO.InputInterface")
QT_END_NAMESPACE