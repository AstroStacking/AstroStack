#pragma once

#include <IO/io.h>
#include <Processing/config.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT AstroImage maxStacking(const std::vector<AstroImage>& imgs);
}
} // namespace astro
