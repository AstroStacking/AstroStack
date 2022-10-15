#pragma once

#include <IO/io.h>
#include <Processing/config.h>

#include <H5Cpp.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT void maxStacking(const H5::DataSet& inputs, H5::DataSet& output);
}
} // namespace astro
