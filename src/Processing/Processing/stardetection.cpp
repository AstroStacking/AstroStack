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

ScalarImageTypePtr starDetection(const ScalarImageTypePtr& inputImg, H5::Group& output, const std::string& dataset,
                                 float threshold, int32_t discardBigger)
{
    using BinaryThresholdImageFilterType = itk::BinaryThresholdImageFilter<ScalarImageType, ScalarIntegerImageType>;
    using ConnectedComponentImageFilterType =
            itk::ConnectedComponentImageFilter<ScalarIntegerImageType, ScalarIntegerImageType>;

    auto segmented = BinaryThresholdImageFilterType::New();
    auto connected = ConnectedComponentImageFilterType::New();

    segmented->SetInput(inputImg);
    segmented->SetUpperThreshold(1);
    segmented->SetLowerThreshold(threshold);
    segmented->SetOutsideValue(0);
    segmented->SetInsideValue(std::numeric_limits<UnderlyingScalarPixelType>::max());
    segmented->Update();
    connected->SetInput(segmented->GetOutput());
    connected->Update();

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
    {
        std::sort(data.begin(), data.end(), [](const auto& r, const auto& l) { return r.back() > l.back(); });
        auto it = std::remove_if(data.begin(), data.end(),
                                 [=](const auto& el) { return el.back() == 0 || el.back() > discardBigger; });
        data.erase(it, data.end());
    }

    // Output data
    hsize_t outputImgDim[2]{data.size() / stats.getComponents(), stats.getComponents()};
    H5::DataSpace outputSpace(2, outputImgDim);
    // output list
    H5::DataSet outputDataset = astro::hdf5::createDataset<double>(dataset, outputSpace, output);
    outputDataset.write(data.data(), H5::PredType::NATIVE_DOUBLE, outputSpace, outputSpace);

    using FilterType = itk::MultiplyImageFilter<ScalarIntegerImageType, ScalarImageType, ScalarImageType>;
    auto filter = FilterType::New();
    filter->SetInput(segmented->GetOutput());
    filter->SetConstant(1 / static_cast<UnderlyingPixelType>(std::numeric_limits<UnderlyingScalarPixelType>::max()));
    filter->Update();

    return filter->GetOutput();
}
} // namespace

ScalarImageTypePtr starDetection(const H5::DataSet& input, H5::Group& output, const std::string& dataset,
                                 float threshold, int32_t discardBigger)
{
    auto inputImg = astro::hdf5::extractScalarFrom(input);

    return starDetection(inputImg, output, dataset, threshold, discardBigger);
}

ScalarImageTypePtr starDetection(const H5::DataSet& input, size_t index, H5::Group& output, const std::string& dataset,
                                 float threshold, int32_t discardBigger)
{
    auto inputImg = astro::hdf5::extractScalarFrom(input, index);

    return starDetection(inputImg, output, dataset, threshold, discardBigger);
}

} // namespace processing
} // namespace astro
