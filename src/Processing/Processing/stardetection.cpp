#include "stardetection.h"

#include <Algos/Filters/Stackers/max.h>
#include <Algos/Filters/multifunctorimagefilter.h>
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

    // compute star stats

    // Output data
    //hsize_t outputImgDim[3]{2, properties.size(), nbDims};
    //H5::DataSpace outputSpace(3, outputImgDim);
    // output list
    //H5::DataSet outputDataset =
    //        astro::hdf5::createDataset(outputDatasetName, outputSpace, H5::PredType::NATIVE_INT64, h5file);

    using FilterType = itk::MultiplyImageFilter<ScalarIntegerImageType, ScalarImageType, ScalarImageType>;
    auto filter = FilterType::New();
    filter->SetInput(segmented->GetOutput());
    filter->SetConstant(1 / static_cast<UnderlyingPixelType>(std::numeric_limits<UnderlyingScalarPixelType>::max()));
    filter->Update();

    return filter->GetOutput();
}
} // namespace processing
} // namespace astro
