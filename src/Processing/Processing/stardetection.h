#pragma once

#include <IO/io.h>
#include <Processing/config.h>

#include <H5Cpp.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ScalarImageTypePtr starDetection(const H5::DataSet& input, H5::Group& output,
                                                         const std::string& dataset, int32_t minStars,
                                                         int32_t maxStars);
ASTRO_PROCESSING_EXPORT ScalarImageTypePtr starDetection(const H5::DataSet& input, size_t index, H5::Group& output,
                                                         const std::string& dataset, int32_t minStars,
                                                         int32_t maxStars);
} // namespace processing
} // namespace astro
