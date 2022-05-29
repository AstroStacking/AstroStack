#include "histostretch.h"
#include "ui_histostretch.h"
#include "ui_mono.h"

namespace astro
{
HistoStretch::~HistoStretch() = default;

QString HistoStretch::name() const
{
    return "Exponential";
}

QString HistoStretch::explanation() const
{
    return tr("Raises the image values to the power of a parameter");
}

MonoInterfaceGUI* HistoStretch::generateGUI(QWidget* parent) const
{
    return new HistoStretchGUI(parent);
}

HistoStretchGUI::HistoStretchGUI(QWidget* parent)
    : MonoInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::HistoStretch>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_monoUi->setupUi(this, child);
    setTitle(tr("Exponential"));

    setupSlots();
    //connect(m_ui->skew, &QDoubleSpinBox::valueChanged, this, &ExponentialGUI::setSkewValue);
    //connect(m_ui->skewSlider, &QSlider::sliderMoved, this, &ExponentialGUI::setApproximateSkewValue);
}

HistoStretchGUI::~HistoStretchGUI() = default;

ImageTypePtr HistoStretchGUI::process(ImageTypePtr img, QPromise<void>& promise)
{
    emit save(img);
}
} // namespace astro
