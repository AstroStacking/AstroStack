#include "histostretch.h"

#include <itkImageAdaptor.h>
#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>
#include <itkImageToHistogramFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkMultiplyImageFilter.h>

#include <numeric>

namespace astro
{
namespace processing
{
namespace
{
template<typename ImageTypePtr>
std::vector<size_t> histogram(const ImageTypePtr& img, size_t bins)
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
float getMaxHistogram(const ImageTypePtr& img, double ratio)
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

std::array<float, 3> getRelativeLimits(const ImageTypePtr& img, float red, float green, float blue)
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

    return {{getMaxHistogram(indexSelectionFilter0, red), getMaxHistogram(indexSelectionFilter1, green),
             getMaxHistogram(indexSelectionFilter2, blue)}};
}

std::array<float, 3> getLimits(const ImageTypePtr& img, float red, float green, float blue, bool relative)
{
    if (relative)
    {
        return getRelativeLimits(img, red, green, blue);
    }
    return {{red, green, blue}};
}
} // namespace

ImageTypePtr histoStretch(const ImageTypePtr& img, float red, float green, float blue, bool relative)
{
    std::array<float, 3> shift = getLimits(img, red, green, blue, relative);
    using DuplicatorType = itk::ImageDuplicator<ImageType>;
    auto duplicator = DuplicatorType::New();
    duplicator->SetInputImage(img);
    duplicator->Update();

    auto outImage = duplicator->GetOutput();
    using IteratorType = itk::ImageRegionIterator<ImageType>;

    IteratorType it(duplicator->GetOutput(), duplicator->GetOutput()->GetRequestedRegion());
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

    return outImage;
}
} // namespace processing
} // namespace astro
