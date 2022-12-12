#pragma once

#include <IO/io.h>
#include <Processing/config.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ImageTypePtr chromaSmoothing(const ImageTypePtr& img, float variance);
}
} // namespace astro
