#pragma once

#include <Processing/config.h>

#include <H5Cpp.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT void starDetection(const H5::DataSet& input, H5::Group& output, const std::string& dataset);
}
} // namespace astro
