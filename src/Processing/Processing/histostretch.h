#pragma once

#include <Processing/mono.h>

namespace Ui
{
class HistoStretch;
}

namespace astro
{
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
 QWidgets that will be displayed in the stack
 */
class HistoStretchGUI : public MonoInterfaceGUI
{
    Q_OBJECT
public:
    HistoStretchGUI(QWidget* parent);
    ~HistoStretchGUI() override;

    ImageTypePtr process(ImageTypePtr img, QPromise<void>& promise) override;

private slots:
    //    void setSkewValue(double val);
    //    void setApproximateSkewValue(int val);

private:
    std::unique_ptr<Ui::HistoStretch> m_ui;
};
} // namespace astro
