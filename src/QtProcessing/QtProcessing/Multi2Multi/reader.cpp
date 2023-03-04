#include "../ui_multi2multiinterface.h"
#include "maxstacking.h"
#include "ui_maxstacking.h"
#include <Processing/chromasmoothing.h>

#include <QtWidgets/QDoubleSpinBox>

namespace astro
{

MaxStacking::~MaxStacking() = default;

QString MaxStacking::name() const
{
    return "MaxStacking";
}

QString MaxStacking::explanation() const
{
    return tr("Raises the image values to the power of a parameter");
}

Multi2MultiInterfaceGUI* MaxStacking::generateGUI(QWidget* parent) const
{
    return new MaxStackingGUI(parent);
}

MaxStackingGUI::MaxStackingGUI(QWidget* parent)
    : Multi2MultiInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::MaxStacking>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_multiUi->setupUi(this, child);
    setTitle(tr("MaxStacking"));

    //setupSlots();
    connect(m_ui->variance, &QDoubleSpinBox::valueChanged, this, &MaxStackingGUI::setSkewValue);
    connect(m_ui->varianceSlider, &QSlider::valueChanged, this, &MaxStackingGUI::setApproximateSkewValue);
}

MaxStackingGUI::~MaxStackingGUI() = default;

void MaxStackingGUI::setSkewValue(double val)
{
    m_ui->varianceSlider->setValue(static_cast<int>(val));
}

void MaxStackingGUI::setApproximateSkewValue(int val)
{
    m_ui->variance->setValue(val);
}

void MaxStackingGUI::process(H5::Group group, QPromise<void>& promise)
{
    //img.setImg(processing::chromaSmoothing(img.getImg(), m_ui->variance->value()));

    //emit save(img);

    //return img;
}
} // namespace astro
