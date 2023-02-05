#include "exponential.h"
#include "../ui_mono2monointerface.h"
#include "ui_exponential.h"
#include <Processing/exponential.h>

#include <QtWidgets/QDoubleSpinBox>

namespace astro
{

Exponential::~Exponential() = default;

QString Exponential::name() const
{
    return "Exponential";
}

QString Exponential::explanation() const
{
    return tr("Raises the image values to the power of a parameter");
}

MonoInterfaceGUI* Exponential::generateGUI(QWidget* parent) const
{
    return new ExponentialGUI(parent);
}

ExponentialGUI::ExponentialGUI(QWidget* parent)
    : MonoInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::Exponential>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_monoUi->setupUi(this, child);
    setTitle(tr("Exponential"));

    setupSlots();
    connect(m_ui->skew, &QDoubleSpinBox::valueChanged, this, &ExponentialGUI::setSkewValue);
    connect(m_ui->skewSlider, &QSlider::valueChanged, this, &ExponentialGUI::setApproximateSkewValue);
}

ExponentialGUI::~ExponentialGUI() = default;

void ExponentialGUI::setSkewValue(double val)
{
    m_ui->skewSlider->setValue(static_cast<int>(100 * val));
}

void ExponentialGUI::setApproximateSkewValue(int val)
{
    m_ui->skew->setValue(val / 100.);
}

AstroImage ExponentialGUI::process(AstroImage img, QPromise<void>& promise)
{
    img.setImg(processing::exponential(img.getImg(), m_ui->skew->value()));

    emit save(img);

    return img;
}
} // namespace astro
