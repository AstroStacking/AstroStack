#pragma once

#include <Processing/config.h>

#include <H5Cpp.h>

#include <functional>
#include <optional>

namespace astro
{
namespace processing
{
template<typename Stacker>
ASTRO_PROCESSING_EXPORT void
stacking(const H5::DataSet& inputs, H5::DataSet& output, const Stacker& stacker,
         std::optional<std::function<void()>> updateTask = std::optional<std::function<void()>>());
}
} // namespace astro
