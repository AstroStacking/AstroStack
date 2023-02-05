#pragma once
#include <QtProcessing/mono2monointerface.h>

namespace Ui
{
class Exponential;
}

namespace astro
{
/**
 * Raises the image values to the power of a parameter
 */
class Exponential
    : public QObject
    , public Mono2MonoInterface
{
    Q_OBJECT
public:
    ~Exponential() override;

    QString name() const override;

    QString explanation() const override;

    Mono2MonoInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 * QWidgets that will be displayed in the stack
 */
class ExponentialGUI : public Mono2MonoInterfaceGUI
{
    Q_OBJECT
public:
    ExponentialGUI(QWidget* parent);
    ~ExponentialGUI() override;

    AstroImage process(AstroImage img, QPromise<void>& promise) override;

private slots:
    void setSkewValue(double val);
    void setApproximateSkewValue(int val);

private:
    std::unique_ptr<Ui::Exponential> m_ui;
};

} // namespace astro
