#pragma once

#include <IO/io.h>
#include <Processing/config.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ImageTypePtr histoStretch(const ImageTypePtr& img, float red, float green, float blue,
                                                  bool relative);
}
} // namespace astro
