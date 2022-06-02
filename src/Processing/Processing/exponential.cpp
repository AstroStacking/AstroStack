#include "exponential.h"
#include "ui_exponential.h"
#include "ui_mono.h"

#include <QtWidgets/QDoubleSpinBox>

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>

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
    connect(m_ui->skewSlider, &QSlider::sliderMoved, this, &ExponentialGUI::setApproximateSkewValue);
}

ExponentialGUI::~ExponentialGUI() = default;

void ExponentialGUI::setSkewValue(double val)
{
    m_ui->skewSlider->setValue(100 * val);
}

void ExponentialGUI::setApproximateSkewValue(int val)
{
    m_ui->skew->setValue(static_cast<int>(val / 100));
}

ImageTypePtr ExponentialGUI::process(ImageTypePtr img, QPromise<void>& promise)
{
    float exponent = m_ui->skew->value();
    unsigned int nbDims = std::min(3U, img->GetNumberOfComponentsPerPixel());

    using DuplicatorType = itk::ImageDuplicator<ImageType>;
    auto duplicator = DuplicatorType::New();
    duplicator->SetInputImage(img);
    duplicator->Update();

    img = duplicator->GetOutput();
    using IteratorType = itk::ImageRegionIterator<ImageType>;

    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        for (unsigned int i = 0; i < nbDims; ++i)
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
