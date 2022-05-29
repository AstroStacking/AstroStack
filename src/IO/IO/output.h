#pragma once
#include <IO/io.h>

#include <QtWidgets/QWidget>

namespace astro
{

class OutputInterface
{
public:
    virtual ~OutputInterface();

    virtual QStringList filters() const = 0;
    virtual void save(ImageTypePtr image, QString filename, QWidget* parent) const = 0;

    static void saveImg(ImageTypePtr img, QString path, QWidget* parent);
};
} // namespace astro

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(astro::OutputInterface, "org.Astro.IO.OutputInterface")
QT_END_NAMESPACE
