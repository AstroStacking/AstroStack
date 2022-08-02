#pragma once

#include <QtProcessing/monointerface.h>

namespace Ui
{
class HistoStretch;
}

namespace astro
{
/**
 * Stretches the histogram to remove black pixels
 */
class HistoStretch
    : public QObject
    , public MonoInterface
{
    Q_OBJECT
public:
    ~HistoStretch() override;

    QString name() const override;

    QString explanation() const override;

    MonoInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 * QWidgets that will be displayed in the stack
 */
class HistoStretchGUI : public MonoInterfaceGUI
{
    Q_OBJECT
public:
    HistoStretchGUI(QWidget* parent);
    ~HistoStretchGUI() override;

    AstroImage process(AstroImage img, QPromise<void>& promise) override;

private slots:
    void setRedValue(double val);
    void setApproximateRedValue(int val);
    void setGreenValue(double val);
    void setApproximateGreenValue(int val);
    void setBlueValue(double val);
    void setApproximateBlueValue(int val);

private:
    std::unique_ptr<Ui::HistoStretch> m_ui;
};
} // namespace astro
