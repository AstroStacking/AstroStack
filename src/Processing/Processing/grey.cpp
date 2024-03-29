#include "grey.h"

#include <Algos/Filters/Convertors/RGB2HSL.h>
#include <IO/hdf5.h>
#include <IO/traits.h>

#include <itkCastImageFilter.h>
#include <itkImageAdaptor.h>

namespace astro
{
namespace processing
{
ScalarImageTypePtr grey(const H5::DataSet& input, size_t index, const H5::Group& output, const std::string& dataset)
{
    ImageTypePtr img = hdf5::extractFrom(input, index);

    auto outputImg = grey(img);

    hdf5::writeTo(*outputImg, output, dataset);

    return outputImg;
}

ScalarImageTypePtr grey(const ImageTypePtr& img)
{
    using RGB2LConvertor = itk::ImageAdaptor<ImageType, filters::convertors::LPixelAccessor>;
    auto finalImg = RGB2LConvertor::New();
    finalImg->SetImage(img);
    finalImg->Update();

    using CastFilterType = itk::CastImageFilter<RGB2LConvertor, ScalarImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(finalImg);
    castFilter->Update();
    return castFilter->GetOutput();
}
} // namespace processing
} // namespace astro
