#include "chromasmoothing.h"

#include <Algos/Filters/Stackers/max.h>
#include <Algos/Filters/multifunctorimagefilter.h>


namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT AstroImage maxStacking(const std::vector<AstroImage>& imgs)
{
    using StackerFilter = astro::filters::MultiFunctorImageFilter<ImageType, ImageType,
                                                                  astro::filters::stackers::Max<ImageType::PixelType>>;

    auto filter = StackerFilter::New();
    for (size_t i = 0; i < imgs.size(); ++i)
    {
        filter->SetNthInput(i, const_cast<ImageType*>(&*imgs[i].getImg()));
    }

    AstroImage img = imgs[0];
    img.setImg(filter->GetOutput());

    return img;
}
} // namespace processing
} // namespace astro
