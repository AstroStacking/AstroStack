#include "histostretch.h"
#include "ui_histostretch.h"
#include "../ui_monointerface.h"
#include <Processing/histostretch.h>

namespace astro
{
HistoStretch::~HistoStretch() = default;

QString HistoStretch::name() const
{
    return "HistoStretch";
}

QString HistoStretch::explanation() const
{
    return tr("Stretches the histogram to maximise low light contrast");
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
    setTitle(tr("Histogram Stretch"));

    setupSlots();
    connect(m_ui->red, &QDoubleSpinBox::valueChanged, this, &HistoStretchGUI::setRedValue);
    connect(m_ui->redSlider, &QSlider::valueChanged, this, &HistoStretchGUI::setApproximateRedValue);
    connect(m_ui->blue, &QDoubleSpinBox::valueChanged, this, &HistoStretchGUI::setBlueValue);
    connect(m_ui->blueSlider, &QSlider::valueChanged, this, &HistoStretchGUI::setApproximateBlueValue);
    connect(m_ui->green, &QDoubleSpinBox::valueChanged, this, &HistoStretchGUI::setGreenValue);
    connect(m_ui->greenSlider, &QSlider::valueChanged, this, &HistoStretchGUI::setApproximateGreenValue);
}

HistoStretchGUI::~HistoStretchGUI() = default;

void HistoStretchGUI::setRedValue(double val)
{
    m_ui->redSlider->setValue(val * 100);
}

void HistoStretchGUI::setApproximateRedValue(int val)
{
    m_ui->red->setValue(val / 100.);
}

void HistoStretchGUI::setBlueValue(double val)
{
    m_ui->blueSlider->setValue(val * 100);
}

void HistoStretchGUI::setApproximateBlueValue(int val)
{
    m_ui->blue->setValue(val / 100.);
}

void HistoStretchGUI::setGreenValue(double val)
{
    m_ui->greenSlider->setValue(val * 100);
}

void HistoStretchGUI::setApproximateGreenValue(int val)
{
    m_ui->green->setValue(val / 100.);
}

AstroImage HistoStretchGUI::process(AstroImage img, QPromise<void>& promise)
{
    img.setImg(processing::histoStretch(img.getImg(), m_ui->red->value(), m_ui->green->value(), m_ui->blue->value(),
                                        m_ui->relative->isChecked()));

    emit save(img);

    return img;
}
} // namespace astro
