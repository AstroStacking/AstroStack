#pragma once

#include <IO/io.h>
#include <Processing/config.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ImageTypePtr exponential(const ImageTypePtr& img, float exponent);
}
} // namespace astro
