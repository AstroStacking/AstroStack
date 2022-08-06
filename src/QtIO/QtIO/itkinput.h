#pragma once
#include <QtIO/input.h>

namespace astro
{

class ITKInputPlugin
    : public QObject
    , public InputInterface
{
    Q_OBJECT
    Q_INTERFACES(astro::InputInterface)

public:
    ~ITKInputPlugin() override;
    QStringList filters() const override;
    ImageTypePtr open(QString filename, QWidget* parent) const override;
};

} // namespace astro
