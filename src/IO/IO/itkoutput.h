#pragma once
#include <IO/output.h>

namespace astro
{

class ITKOutputPlugin
    : public QObject
    , public OutputInterface
{
    Q_OBJECT
    Q_INTERFACES(astro::OutputInterface)

public:
    ITKOutputPlugin();
    ~ITKOutputPlugin() override;
    QStringList filters() const override;
    void save(ImageTypePtr img, QString filename, QWidget* parent) const override;
};

} // namespace astro
