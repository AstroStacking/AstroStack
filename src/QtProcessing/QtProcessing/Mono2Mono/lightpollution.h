#pragma once
#include <QtProcessing/mono2monointerface.h>

namespace Ui
{
class LightPollution;
}

namespace astro
{
/**
 * Estimates and removes the light pollution
 */
class LightPollution
    : public QObject
    , public Mono2MonoInterface
{
    Q_OBJECT
public:
    ~LightPollution() override;

    QString name() const override;

    QString explanation() const override;

    Mono2MonoInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 * QWidgets that will be displayed in the stack
 */
class LightPollutionGUI : public Mono2MonoInterfaceGUI
{
    Q_OBJECT
public:
    LightPollutionGUI(QWidget* parent);
    ~LightPollutionGUI() override;

    AstroImage process(AstroImage img, QPromise<void>& promise) override;

private slots:

private:
    std::unique_ptr<Ui::LightPollution> m_ui;
};

} // namespace astro
