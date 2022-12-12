#include "lightpollution.h"

#include <Algos/gradient_estimation.h>

#include <itkSubtractImageFilter.h>

namespace astro
{
namespace processing
{
ImageTypePtr lightPollution(const ImageTypePtr& img)
{
    ImageTypePtr light = estimateGradient(img);

    using SubtractImageFilterType = itk::SubtractImageFilter<ImageType, ImageType>;

    auto subtractFilter = SubtractImageFilterType::New();
    subtractFilter->SetInput1(img);
    subtractFilter->SetInput2(light);
    subtractFilter->Update();

    return subtractFilter->GetOutput();
}
} // namespace processing
} // namespace astro
