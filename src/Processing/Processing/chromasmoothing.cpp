#include "chromasmoothing.h"
#include "ui_chromasmoothing.h"
#include "ui_monointerface.h"

#include <QtWidgets/QDoubleSpinBox>

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>

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

MonoInterfaceGUI* ChromaSmoothing::generateGUI(QWidget* parent) const
{
    return new ChromaSmoothingGUI(parent);
}

ChromaSmoothingGUI::ChromaSmoothingGUI(QWidget* parent)
    : MonoInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::ChromaSmoothing>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_monoUi->setupUi(this, child);
    setTitle(tr("ChromaSmoothing"));

    setupSlots();
    connect(m_ui->skew, &QDoubleSpinBox::valueChanged, this, &ChromaSmoothingGUI::setSkewValue);
    connect(m_ui->skewSlider, &QSlider::valueChanged, this, &ChromaSmoothingGUI::setApproximateSkewValue);
}

ChromaSmoothingGUI::~ChromaSmoothingGUI() = default;

void ChromaSmoothingGUI::setSkewValue(double val)
{
    m_ui->skewSlider->setValue(static_cast<int>(100 * val));
}

void ChromaSmoothingGUI::setApproximateSkewValue(int val)
{
    m_ui->skew->setValue(val / 100.);
}

AstroImage ChromaSmoothingGUI::process(AstroImage img, QPromise<void>& promise)
{
    float exponent = m_ui->skew->value();

    using DuplicatorType = itk::ImageDuplicator<ImageType>;
    auto duplicator = DuplicatorType::New();
    duplicator->SetInputImage(img.getImg());
    duplicator->Update();

    img.setImg(duplicator->GetOutput());
    using IteratorType = itk::ImageRegionIterator<ImageType>;

    IteratorType it(img.getImg(), img.getImg()->GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        for (unsigned int i = 0; i < PixelDimension; ++i)
        {
            value.SetElement(i, std::pow(value.GetElement(i), exponent));
        }
        it.Set(value);
        ++it;
    }

    emit save(img);

    return img;
}
} // namespace astro
