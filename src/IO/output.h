#pragma once
#include <io.h>

#include <QtWidgets/QWidget>

namespace astro
{

class OutputInterface
{
public:
    virtual ~OutputInterface();

    virtual QStringList filters() const = 0;
    virtual void save(const ImageType& image, QString filename, QWidget* parent) = 0;
};
} // namespace astro

QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(astro::OutputInterface, "org.Astro.IO.OutputInterface")
QT_END_NAMESPACE
