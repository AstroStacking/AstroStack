#include "stardetection.h"

#include <Algos/ImageStats/center.h>
#include <Algos/ImageStats/image_statistics.h>
#include <Algos/ImageStats/mean.h>
#include <Algos/ImageStats/minmax_position.h>
#include <Algos/ImageStats/size.h>
#include <IO/hdf5.h>
#include <IO/traits.h>

#include <itkBinaryThresholdImageFilter.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkMultiplyImageFilter.h>

namespace astro
{
namespace processing
{
namespace
{
constexpr int MAX_ITERATIONS{50};
}

ScalarImageTypePtr starDetection(const H5::DataSet& input, H5::Group& output, const std::string& dataset,
                                 int32_t minStars, int32_t maxStars)
{
    auto inputImg = astro::hdf5::extractScalarFrom(input);

    using BinaryThresholdImageFilterType = itk::BinaryThresholdImageFilter<ScalarImageType, ScalarIntegerImageType>;
    using ConnectedComponentImageFilterType =
            itk::ConnectedComponentImageFilter<ScalarIntegerImageType, ScalarIntegerImageType>;

    float threshold = 0.5;
    auto segmented = BinaryThresholdImageFilterType::New();
    auto connected = ConnectedComponentImageFilterType::New();

    segmented->SetInput(inputImg);
    segmented->SetUpperThreshold(1);
    segmented->SetOutsideValue(0);
    segmented->SetInsideValue(std::numeric_limits<UnderlyingScalarPixelType>::max());
    connected->SetInput(segmented->GetOutput());

    int counter = 0;
    while (true)
    {
        segmented->SetLowerThreshold(threshold);
        segmented->Update();
        connected->Update();

        if (connected->GetObjectCount() < minStars)
        {
            threshold *= .9;
        }
        else if (connected->GetObjectCount() > maxStars)
        {
            threshold *= 1.05;
            threshold = std::min(threshold, 1.f);
        }
        else
        {
            break;
        }
        if (counter > MAX_ITERATIONS && connected->GetObjectCount() > minStars)
        {
            break;
        }
        ++counter;
    }

    using IntegerIteratorType = itk::ImageRegionIterator<ScalarIntegerImageType>;
    using IteratorType = itk::ImageRegionIterator<ScalarImageType>;
    using Stats = image_statistics::ImageStatistics<image_statistics::Center, image_statistics::Mean,
                                                    image_statistics::MinMaxPosition, image_statistics::Size>;
    Stats stats(connected->GetObjectCount());

    IntegerIteratorType integerIt(connected->GetOutput(), connected->GetOutput()->GetRequestedRegion());
    IteratorType it(inputImg, inputImg->GetRequestedRegion());
    it.GoToBegin();
    integerIt.GoToBegin();

    while (!it.IsAtEnd())
    {
        stats.process(it.Get(), integerIt.Get(), it.GetIndex());
        ++it;
        ++integerIt;
    }

    stats.compute();
    auto data = stats.getData();
    std::sort(data.begin(), data.end(), [](const auto& r, const auto& l) { return r.back() > l.back(); });

    // Output data
    hsize_t outputImgDim[2]{stats.getSize(), stats.getComponents()};
    H5::DataSpace outputSpace(2, outputImgDim);
    // output list
    H5::DataSet outputDataset = output.createDataSet(dataset, H5::PredType::NATIVE_DOUBLE, outputSpace);
    outputDataset.write(data.data(), H5::PredType::NATIVE_DOUBLE, outputSpace, outputSpace);

    using FilterType = itk::MultiplyImageFilter<ScalarIntegerImageType, ScalarImageType, ScalarImageType>;
    auto filter = FilterType::New();
    filter->SetInput(segmented->GetOutput());
    filter->SetConstant(1 / static_cast<UnderlyingPixelType>(std::numeric_limits<UnderlyingScalarPixelType>::max()));
    filter->Update();

    return filter->GetOutput();
}
} // namespace processing
} // namespace astro
