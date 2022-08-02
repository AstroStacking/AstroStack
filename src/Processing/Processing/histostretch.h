#pragma once

#include <IO/io.h>
#include <Processing/config.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT AstroImage histoStretch(AstroImage img, float red, float blue, float green, bool relative);
}
} // namespace astro
