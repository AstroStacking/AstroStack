#pragma once

#include <QtProcessing/mono2monointerface.h>

namespace Ui
{
class RLDeconvolution;
}

namespace astro
{
/**
 * Richardson Lucy Deconvolution
 */
class RLDeconvolution
    : public QObject
    , public Mono2MonoInterface
{
    Q_OBJECT
public:
    ~RLDeconvolution() override;

    QString name() const override;

    QString explanation() const override;

    Mono2MonoInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 * QWidgets that will be displayed in the stack
 */
class RLDeconvolutionGUI : public Mono2MonoInterfaceGUI
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
