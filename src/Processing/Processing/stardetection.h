#pragma once

#include <IO/io.h>
#include <Processing/config.h>

#include <H5Cpp.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ScalarImageTypePtr starDetection(const H5::DataSet& input, H5::Group& output,
                                                         const std::string& dataset, float threshold,
                                                         int32_t discardBigger);
ASTRO_PROCESSING_EXPORT ScalarImageTypePtr starDetection(const H5::DataSet& input, size_t index, H5::Group& output,
                                                         const std::string& dataset, float threshold,
                                                         int32_t discardBigger);
} // namespace processing
} // namespace astro
