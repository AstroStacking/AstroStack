#include "histostretch.h"
#include "ui_histostretch.h"
#include "ui_monointerface.h"

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>
#include <itkImageToHistogramFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkMultiplyImageFilter.h>
#include <itkVectorIndexSelectionCastImageFilter.h>

namespace astro
{
HistoStretch::~HistoStretch() = default;

QString HistoStretch::name() const
{
    return "HistoStretch";
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
    setTitle(tr("Histogram Stretch"));

    setupSlots();
    connect(m_ui->red, &QDoubleSpinBox::valueChanged, this, &HistoStretchGUI::setRedValue);
    connect(m_ui->redSlider, &QSlider::sliderMoved, this, &HistoStretchGUI::setApproximateRedValue);
    connect(m_ui->blue, &QDoubleSpinBox::valueChanged, this, &HistoStretchGUI::setBlueValue);
    connect(m_ui->blueSlider, &QSlider::sliderMoved, this, &HistoStretchGUI::setApproximateBlueValue);
    connect(m_ui->green, &QDoubleSpinBox::valueChanged, this, &HistoStretchGUI::setGreenValue);
    connect(m_ui->greenSlider, &QSlider::sliderMoved, this, &HistoStretchGUI::setApproximateGreenValue);
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

float HistoStretchGUI::getMaxHistogram(ScalarImageTypePtr img)
{
    constexpr unsigned int BINS = 65536;
    constexpr unsigned int MEAN_SIZE = 5;
    using ImageToHistogramFilterType = itk::Statistics::ImageToHistogramFilter<ScalarImageType>;

    ImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(BINS);
    lowerBound.Fill(0);

    ImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(BINS);
    upperBound.Fill(1);

    ImageToHistogramFilterType::HistogramType::SizeType size(1);
    size.Fill(BINS);

    auto imageToHistogramFilter = ImageToHistogramFilterType::New();
    imageToHistogramFilter->SetInput(img);
    imageToHistogramFilter->SetHistogramBinMinimum(lowerBound);
    imageToHistogramFilter->SetHistogramBinMaximum(upperBound);
    imageToHistogramFilter->SetHistogramSize(size);
    imageToHistogramFilter->Update();
    ImageToHistogramFilterType::HistogramType* histogram = imageToHistogramFilter->GetOutput();

    std::vector<size_t> hist(BINS, 0);

    for (size_t i = 0; i < histogram->GetSize()[0]; ++i)
    {
        hist[i] = histogram->GetFrequency(i);
    }
    std::vector<double> histFilt(BINS, 0);
    for (size_t i = MEAN_SIZE; i < BINS - MEAN_SIZE / 2; ++i)
    {
        histFilt[i] = static_cast<double>(std::accumulate(hist.data() + i - MEAN_SIZE / 2,
                                                          hist.data() + i + MEAN_SIZE / 2 + 1, 0)) /
                      MEAN_SIZE;
    }

    return (std::max_element(histFilt.begin(), histFilt.end()) - histFilt.begin()) / static_cast<float>(BINS);
}

std::array<float, 4> HistoStretchGUI::getLimits1D(const ImageTypePtr& img)
{
    using IndexSelectionType = itk::VectorIndexSelectionCastImageFilter<ImageType, ScalarImageType>;
    auto indexSelectionFilter = IndexSelectionType::New();
    indexSelectionFilter->SetIndex(0);
    indexSelectionFilter->SetInput(img);
    indexSelectionFilter->Update();

    float r = getMaxHistogram(indexSelectionFilter->GetOutput());
    return {{r * static_cast<float>(m_ui->red->value()), r * static_cast<float>(m_ui->green->value()),
             r * static_cast<float>(m_ui->blue->value()), 1}};
}

std::array<float, 4> HistoStretchGUI::getLimits3D(const ImageTypePtr& img)
{
    using IndexSelectionType = itk::VectorIndexSelectionCastImageFilter<ImageType, ScalarImageType>;
    auto indexSelectionFilter0 = IndexSelectionType::New();
    indexSelectionFilter0->SetIndex(0);
    indexSelectionFilter0->SetInput(img);
    indexSelectionFilter0->Update();
    auto indexSelectionFilter1 = IndexSelectionType::New();
    indexSelectionFilter1->SetIndex(1);
    indexSelectionFilter1->SetInput(img);
    indexSelectionFilter1->Update();
    auto indexSelectionFilter2 = IndexSelectionType::New();
    indexSelectionFilter2->SetIndex(2);
    indexSelectionFilter2->SetInput(img);
    indexSelectionFilter2->Update();

    return {{getMaxHistogram(indexSelectionFilter0->GetOutput()) * static_cast<float>(m_ui->red->value()),
             getMaxHistogram(indexSelectionFilter1->GetOutput()) * static_cast<float>(m_ui->green->value()),
             getMaxHistogram(indexSelectionFilter2->GetOutput()) * static_cast<float>(m_ui->blue->value()), 1}};
}

std::array<float, 4> HistoStretchGUI::getLimits(const ImageTypePtr& img)
{
    if (m_ui->relative->isChecked())
    {
        if (img->GetNumberOfComponentsPerPixel() == 3)
        {
            return getLimits3D(img);
        }
        return getLimits1D(img);
    }
    return {{static_cast<float>(m_ui->red->value()), static_cast<float>(m_ui->blue->value()),
             static_cast<float>(m_ui->green->value()), 1}};
}

ImageTypePtr HistoStretchGUI::process(ImageTypePtr img, QPromise<void>& promise)
{
    std::array<float, 4> shift = getLimits(img);

    using DuplicatorType = itk::ImageDuplicator<ImageType>;
    auto duplicator = DuplicatorType::New();
    duplicator->SetInputImage(img);
    duplicator->Update();

    img = duplicator->GetOutput();
    using IteratorType = itk::ImageRegionIterator<ImageType>;

    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();

    unsigned int nbDims = img->GetNumberOfComponentsPerPixel();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        for (unsigned int i = 0; i < nbDims; ++i)
        {
            value.SetElement(i, std::max((value.GetElement(i) - shift[i]) / (1 - shift[i]), 0.f));
        }
        it.Set(value);
        ++it;
    }

    emit save(img);

    return img;
}
} // namespace astro
