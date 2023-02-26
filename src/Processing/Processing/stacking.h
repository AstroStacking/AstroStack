#pragma once

#include <Processing/config.h>

#include <H5Cpp.h>

namespace astro
{
namespace processing
{
template<typename Stacker>
ASTRO_PROCESSING_EXPORT void stacking(const H5::DataSet& inputs, H5::DataSet& output);
}
} // namespace astro
