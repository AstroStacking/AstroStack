#pragma once

#include <IO/io.h>
#include <Processing/config.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT AstroImage chromaSmoothing(AstroImage img, float variance);
}
} // namespace astro
