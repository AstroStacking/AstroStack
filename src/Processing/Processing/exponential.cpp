#include "exponential.h"

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT AstroImage exponential(AstroImage img, float exponent)
{
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
            value.SetElement(i, std::pow(value.GetElement(i), exponent));
        }
        it.Set(value);
        ++it;
    }

    return img;
}
} // namespace processing
} // namespace astro
