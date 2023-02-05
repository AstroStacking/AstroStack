#include "chromasmoothing.h"
#include "../ui_mono2monointerface.h"
#include "ui_chromasmoothing.h"
#include <Processing/chromasmoothing.h>

#include <QtWidgets/QDoubleSpinBox>

namespace astro
{

ChromaSmoothing::~ChromaSmoothing() = default;

QString ChromaSmoothing::name() const
{
    return "ChromaSmoothing";
}

QString ChromaSmoothing::explanation() const
{
    return tr("Raises the image values to the power of a parameter");
}

Mono2MonoInterfaceGUI* ChromaSmoothing::generateGUI(QWidget* parent) const
{
    return new ChromaSmoothingGUI(parent);
}

ChromaSmoothingGUI::ChromaSmoothingGUI(QWidget* parent)
    : Mono2MonoInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::ChromaSmoothing>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_monoUi->setupUi(this, child);
    setTitle(tr("ChromaSmoothing"));

    setupSlots();
    connect(m_ui->variance, &QDoubleSpinBox::valueChanged, this, &ChromaSmoothingGUI::setSkewValue);
    connect(m_ui->varianceSlider, &QSlider::valueChanged, this, &ChromaSmoothingGUI::setApproximateSkewValue);
}

ChromaSmoothingGUI::~ChromaSmoothingGUI() = default;

void ChromaSmoothingGUI::setSkewValue(double val)
{
    m_ui->varianceSlider->setValue(static_cast<int>(val));
}

void ChromaSmoothingGUI::setApproximateSkewValue(int val)
{
    m_ui->variance->setValue(val);
}

AstroImage ChromaSmoothingGUI::process(AstroImage img, QPromise<void>& promise)
{
    img.setImg(processing::chromaSmoothing(img.getImg(), m_ui->variance->value()));

    emit save(img);

    return img;
}
} // namespace astro
