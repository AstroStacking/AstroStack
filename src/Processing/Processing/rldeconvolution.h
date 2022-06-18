#pragma once

#include <Processing/monointerface.h>

namespace Ui
{
class RLDeconvolution;
}

namespace astro
{
class RLDeconvolution
    : public QObject
    , public MonoInterface
{
    Q_OBJECT
public:
    ~RLDeconvolution() override;

    QString name() const override;

    QString explanation() const override;

    MonoInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 QWidgets that will be displayed in the stack
 */
class RLDeconvolutionGUI : public MonoInterfaceGUI
{
    Q_OBJECT
public:
    RLDeconvolutionGUI(QWidget* parent);
    ~RLDeconvolutionGUI() override;

    AstroImage process(AstroImage img, QPromise<void>& promise) override;

private:
    std::unique_ptr<Ui::RLDeconvolution> m_ui;
};
} // namespace astro
