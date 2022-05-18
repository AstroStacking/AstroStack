#pragma once
#include <input.h>

namespace astro
{

class ITKInputPlugin : public QObject, public InputInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.Astro.IO.InputInterface")
    Q_INTERFACES(astro::InputInterface)

public:
    ITKInputPlugin();
    ~ITKInputPlugin() override;
    QStringList filters() const override;
    ImageTypePtr open(QString filename, QWidget* parent) const override;
};

}

