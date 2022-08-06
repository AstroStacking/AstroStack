#pragma once
#include <QtIO/output.h>

namespace astro
{

class ITKOutputPlugin
    : public QObject
    , public OutputInterface
{
    Q_OBJECT
    Q_INTERFACES(astro::OutputInterface)

public:
    ~ITKOutputPlugin() override;
    QStringList filters() const override;
    void save(ImageTypePtr img, QString filename, QWidget* parent) const override;
};

} // namespace astro
