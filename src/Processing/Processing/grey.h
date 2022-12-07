#pragma once

#include <IO/io.h>
#include <Processing/config.h>

#include <H5Cpp.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ImageTypePtr grey(const H5::DataSet& input, size_t index, H5::Group& output,
                                          const std::string& dataset);
}
} // namespace astro
