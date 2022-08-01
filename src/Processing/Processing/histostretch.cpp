#include "histostretch.h"
#include "ui_histostretch.h"
#include "ui_monointerface.h"

#include <itkImageAdaptor.h>
#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>
#include <itkImageToHistogramFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkMultiplyImageFilter.h>

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

template<typename ImageTypePtr>
std::vector<size_t> HistoStretchGUI::histogram(const ImageTypePtr& img, size_t bins)
{
    using ImageToHistogramFilterType = itk::Statistics::ImageToHistogramFilter<typename ImageTypePtr::ObjectType>;

    typename ImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(bins);
    lowerBound.Fill(0);

    typename ImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(bins);
    upperBound.Fill(1);

    typename ImageToHistogramFilterType::HistogramType::SizeType size(1);
    size.Fill(bins);

    auto imageToHistogramFilter = ImageToHistogramFilterType::New();
    imageToHistogramFilter->SetInput(img);
    imageToHistogramFilter->SetHistogramBinMinimum(lowerBound);
    imageToHistogramFilter->SetHistogramBinMaximum(upperBound);
    imageToHistogramFilter->SetHistogramSize(size);
    imageToHistogramFilter->Update();
    typename ImageToHistogramFilterType::HistogramType* histogram = imageToHistogramFilter->GetOutput();

    std::vector<size_t> hist(bins, 0);

    for (size_t i = 0; i < histogram->GetSize()[0]; ++i)
    {
        hist[i] = histogram->GetFrequency(i);
    }
    return hist;
}

template<typename ImageTypePtr>
float HistoStretchGUI::getMaxHistogram(const ImageTypePtr& img, double ratio)
{
    constexpr unsigned int BINS = 256;
    constexpr unsigned int MEAN_SIZE = 5;
    std::vector<size_t> hist = histogram(img, BINS);
    std::vector<double> histFilt(BINS, 0);
    for (size_t i = MEAN_SIZE; i < BINS - MEAN_SIZE / 2; ++i)
    {
        histFilt[i] = static_cast<double>(std::accumulate(hist.data() + i - MEAN_SIZE / 2,
                                                          hist.data() + i + MEAN_SIZE / 2 + 1, 0)) /
                      MEAN_SIZE;
    }

    auto maxElement = std::max_element(histFilt.begin(), histFilt.end());
    double limit = ratio * *maxElement;
    auto justBelow = std::find_if(std::vector<double>::reverse_iterator(maxElement), histFilt.rend(),
                                  [=](double el) { return el < limit; });

    return (justBelow.base() - histFilt.begin()) / static_cast<float>(BINS);
}

std::array<float, 4> HistoStretchGUI::getRelativeLimits(const ImageTypePtr& img)
{
    using IndexSelectionRed = itk::ImageAdaptor<ImageType, RedChannelPixelAccessor>;
    auto indexSelectionFilter0 = IndexSelectionRed::New();
    indexSelectionFilter0->SetImage(img);
    using IndexSelectionGreen = itk::ImageAdaptor<ImageType, GreenChannelPixelAccessor>;
    auto indexSelectionFilter1 = IndexSelectionGreen::New();
    indexSelectionFilter1->SetImage(img);
    using IndexSelectionBlue = itk::ImageAdaptor<ImageType, BlueChannelPixelAccessor>;
    auto indexSelectionFilter2 = IndexSelectionBlue::New();
    indexSelectionFilter2->SetImage(img);

    return {{getMaxHistogram(indexSelectionFilter0, m_ui->red->value()),
             getMaxHistogram(indexSelectionFilter1, m_ui->green->value()),
             getMaxHistogram(indexSelectionFilter2, m_ui->blue->value()), 1}};
}

std::array<float, 4> HistoStretchGUI::getLimits(const ImageTypePtr& img)
{
    if (m_ui->relative->isChecked())
    {
        return getRelativeLimits(img);
    }
    return {{static_cast<float>(m_ui->red->value()), static_cast<float>(m_ui->blue->value()),
             static_cast<float>(m_ui->green->value()), 1}};
}

AstroImage HistoStretchGUI::process(AstroImage img, QPromise<void>& promise)
{
    std::array<float, 4> shift = getLimits(img.getImg());

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
            value.SetElement(i, std::max((value.GetElement(i) - shift[i]) / (1 - shift[i]), 0.f));
        }
        it.Set(value);
        ++it;
    }

    emit save(img);

    return img;
}
} // namespace astro
