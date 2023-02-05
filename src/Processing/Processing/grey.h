#pragma once

#include <IO/io.h>
#include <Processing/config.h>

#include <H5Cpp.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ScalarImageTypePtr grey(const H5::DataSet& input, size_t index, const H5::Group& output,
                                                const std::string& dataset);
ASTRO_PROCESSING_EXPORT ScalarImageTypePtr grey(const ImageTypePtr& img);
} // namespace processing
} // namespace astro
