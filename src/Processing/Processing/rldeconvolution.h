#pragma once

#include <IO/io.h>
#include <Processing/config.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ImageTypePtr RLDeconvolution(const ImageTypePtr& img, const ScalarImageTypePtr& kernel);
ASTRO_PROCESSING_EXPORT ImageTypePtr RLDeconvolution(const ImageTypePtr& img, int filterSize, float sigma);
} // namespace processing
} // namespace astro
