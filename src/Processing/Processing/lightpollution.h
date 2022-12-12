#pragma once

#include <IO/io.h>
#include <Processing/config.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ImageTypePtr lightPollution(const ImageTypePtr& img);
}
} // namespace astro
