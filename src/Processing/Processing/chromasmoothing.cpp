#include "chromasmoothing.h"
#include "ui_chromasmoothing.h"
#include "ui_monointerface.h"

#include <Algos/Filters/Convertors/HSL2RGB.h>
#include <Algos/Filters/Convertors/RGB2HSL.h>
#include <Algos/Filters/mergehslfilter.h>

#include <QtWidgets/QDoubleSpinBox>

#include <itkComposeImageFilter.h>
#include <itkVectorIndexSelectionCastImageFilter.h>
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

    using IndexSelectionType = itk::VectorIndexSelectionCastImageFilter<ImageType, ScalarImageType>;
    using filterType = itk::DiscreteGaussianImageFilter<ScalarImageType, ScalarImageType>;
    using ComposeFilterType = itk::ComposeImageFilter<ScalarImageType, ImageType>;

    auto indexSelectionFilter0 = IndexSelectionType::New();
    auto indexSelectionFilter1 = IndexSelectionType::New();
    auto indexSelectionFilter2 = IndexSelectionType::New();

    indexSelectionFilter0->SetIndex(0);
    indexSelectionFilter0->SetInput(img.getImg());
    indexSelectionFilter0->Update();
    indexSelectionFilter1->SetIndex(1);
    indexSelectionFilter1->SetInput(img.getImg());
    indexSelectionFilter1->Update();
    indexSelectionFilter2->SetIndex(2);
    indexSelectionFilter2->SetInput(img.getImg());
    indexSelectionFilter2->Update();

    // Create and setup a Gaussian filter
    auto gaussianFilter0 = filterType::New();
    auto gaussianFilter1 = filterType::New();
    auto gaussianFilter2 = filterType::New();
    gaussianFilter0->SetInput(indexSelectionFilter0->GetOutput());
    gaussianFilter0->SetVariance(variance);
    gaussianFilter0->Update();
    gaussianFilter1->SetInput(indexSelectionFilter1->GetOutput());
    gaussianFilter1->SetVariance(variance);
    gaussianFilter1->Update();
    gaussianFilter2->SetInput(indexSelectionFilter2->GetOutput());
    gaussianFilter2->SetVariance(variance);
    gaussianFilter2->Update();

    auto composeFilter = ComposeFilterType::New();
    composeFilter->SetInput1(gaussianFilter0->GetOutput());
    composeFilter->SetInput2(gaussianFilter1->GetOutput());
    composeFilter->SetInput3(gaussianFilter2->GetOutput());
    composeFilter->Update();

    using RGB2HSLConvertor = itk::ImageAdaptor<ImageType, filters::convertors::HSLPixelAccessor>;
    auto originalImg = RGB2HSLConvertor::New();
    auto smoothImg = RGB2HSLConvertor::New();
    originalImg->SetImage(img.getImg());
    smoothImg->SetImage(composeFilter->GetOutput());
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
