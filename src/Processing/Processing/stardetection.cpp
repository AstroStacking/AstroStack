#include "stardetection.h"

#include <Algos/Filters/Stackers/max.h>
#include <Algos/Filters/multifunctorimagefilter.h>
#include <IO/hdf5.h>
#include <IO/traits.h>

#include <itkBinaryThresholdImageFilter.h>
#include <itkConnectedComponentImageFilter.h>

namespace astro
{
namespace processing
{
void starDetection(const H5::DataSet& input, H5::DataSet& output, const std::string& dataset, int32_t minStars,
                   int32_t maxStars)
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
    segmented->SetInsideValue(1);
    connected->SetInput(segmented->GetOutput());

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
        }
        else
        {
            break;
        }
        
    }

    // compute star stats

    // Output data
    //hsize_t outputImgDim[3]{2, properties.size(), nbDims};
    //H5::DataSpace outputSpace(3, outputImgDim);
    // output list
    //H5::DataSet outputDataset =
    //        astro::hdf5::createDataset(outputDatasetName, outputSpace, H5::PredType::NATIVE_INT64, h5file);
}
} // namespace processing
} // namespace astro
