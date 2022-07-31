#include "chromasmoothing.h"
#include "ui_chromasmoothing.h"
#include "ui_monointerface.h"

#include <Algos/Filters/Convertors/HSL2RGB.h>
#include <Algos/Filters/Convertors/RGB2HSL.h>
#include <Algos/Filters/mergehslfilter.h>

#include <QtWidgets/QDoubleSpinBox>

#include <itkCastImageFilter.h>
#include <itkDiscreteGaussianImageFilter.h>
#include <itkImageAdaptor.h>

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
    connect(m_ui->variance, &QDoubleSpinBox::valueChanged, this, &ChromaSmoothingGUI::setSkewValue);
    connect(m_ui->varianceSlider, &QSlider::valueChanged, this, &ChromaSmoothingGUI::setApproximateSkewValue);
}

ChromaSmoothingGUI::~ChromaSmoothingGUI() = default;

void ChromaSmoothingGUI::setSkewValue(double val)
{
    m_ui->varianceSlider->setValue(static_cast<int>(100 * val));
}

void ChromaSmoothingGUI::setApproximateSkewValue(int val)
{
    m_ui->variance->setValue(val / 100.);
}

AstroImage ChromaSmoothingGUI::process(AstroImage img, QPromise<void>& promise)
{
    float variance = m_ui->variance->value();

    using filterType = itk::DiscreteGaussianImageFilter<ImageType, ImageType>;

    // Create and setup a Gaussian filter
    auto gaussianFilter = filterType::New();
    gaussianFilter->SetInput(img.getImg());
    gaussianFilter->SetVariance(variance);
    gaussianFilter->Update();

    using RGB2HSLConvertor = itk::ImageAdaptor<ImageType, filters::convertors::HSLPixelAccessor>;
    auto originalImg = RGB2HSLConvertor::New();
    auto smoothImg = RGB2HSLConvertor::New();
    originalImg->SetImage(img.getImg());
    smoothImg->SetImage(gaussianFilter->GetOutput());
    originalImg->Update();
    smoothImg->Update();

    using MergeFilter = astro::filters::MergeHSLFilter<RGB2HSLConvertor, RGB2HSLConvertor, ImageType>;
    auto mergeFilter = MergeFilter::New();
    mergeFilter->SetInput1(originalImg);
    mergeFilter->SetInput2(smoothImg);
    mergeFilter->Update();

    using HSL2RGBConvertor = itk::ImageAdaptor<ImageType, filters::convertors::RGBPixelAccessor>;
    auto finalImg = HSL2RGBConvertor::New();
    finalImg->SetImage(mergeFilter->GetOutput());
    finalImg->Update();

    using CastFilterType = itk::CastImageFilter<HSL2RGBConvertor, ImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(finalImg);
    castFilter->Update();
    
    img.setImg(castFilter->GetOutput());

    emit save(img);

    return img;
}
} // namespace astro
