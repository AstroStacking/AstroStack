#pragma once
#include <QtProcessing/mono2monointerface.h>

namespace Ui
{
class ChromaSmoothing;
}

namespace astro
{
/**
 * Raises the image values to the power of a parameter
 */
class ChromaSmoothing
    : public QObject
    , public MonoInterface
{
    Q_OBJECT
public:
    ~ChromaSmoothing() override;

    QString name() const override;

    QString explanation() const override;

    MonoInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 QWidgets that will be displayed in the stack
 */
class ChromaSmoothingGUI : public MonoInterfaceGUI
{
    Q_OBJECT
public:
    ChromaSmoothingGUI(QWidget* parent);
    ~ChromaSmoothingGUI() override;

    AstroImage process(AstroImage img, QPromise<void>& promise) override;

private slots:
    void setSkewValue(double val);
    void setApproximateSkewValue(int val);

private:
    std::unique_ptr<Ui::ChromaSmoothing> m_ui;
};

} // namespace astro
