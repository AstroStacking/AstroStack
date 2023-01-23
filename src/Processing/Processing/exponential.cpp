#include "exponential.h"

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>
#include <itkMultiThreaderBase.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ImageTypePtr exponential(const ImageTypePtr& img, float exponent)
{
    using DuplicatorType = itk::ImageDuplicator<ImageType>;
    auto duplicator = DuplicatorType::New();
    duplicator->SetInputImage(img);
    duplicator->Update();
    auto outImage = duplicator->GetOutput();

    itk::MultiThreaderBase::Pointer mt = itk::MultiThreaderBase::New();
    mt->ParallelizeImageRegion<Dimension>(
            outImage->GetRequestedRegion(),
            [&outImage, exponent](const ImageType::RegionType& region)
            {
                itk::ImageRegionIterator<ImageType> it(outImage, region);
                for (; !it.IsAtEnd(); ++it)
                {
                    auto value = it.Get();
                    for (unsigned int i = 0; i < PixelDimension; ++i)
                    {
                        value.SetElement(i, std::pow(value.GetElement(i), exponent));
                    }
                    it.Set(value);
                    ++it;
                }
            },
            nullptr);
    return outImage;
}
} // namespace processing
} // namespace astro
