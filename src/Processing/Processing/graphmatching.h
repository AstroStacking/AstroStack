#pragma once

#include <IO/io.h>
#include <Processing/config.h>

#include <H5Cpp.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT std::vector<std::pair<size_t, size_t>>
graphmatching(const std::vector<std::pair<double, double>>& graph1,
              const std::vector<std::pair<double, double>>& graph2, int fullGraphMatch, double maxRatio);
}
} // namespace astro
