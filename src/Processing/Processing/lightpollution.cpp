#include "lightpollution.h"

#include <Algos/gradient_estimation.h>

#include <itkSubtractImageFilter.h>

namespace astro
{
namespace processing
{
AstroImage lightPollution(AstroImage img)
{
    ImageTypePtr light = estimateGradient(img.getImg());

    using SubtractImageFilterType = itk::SubtractImageFilter<ImageType, ImageType>;

    auto subtractFilter = SubtractImageFilterType::New();
    subtractFilter->SetInput1(img.getImg());
    subtractFilter->SetInput2(light);
    subtractFilter->Update();

    img.setImg(subtractFilter->GetOutput());

    return img;
}
} // namespace processing
} // namespace astro
