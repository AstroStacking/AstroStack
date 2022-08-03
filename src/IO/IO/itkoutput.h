#pragma once


#include <IO/config.h>
#include <IO/io.h>

namespace astro
{
namespace io
{
ASTRO_IO_EXPORT void save(const ImageTypePtr& img, const std::string& filename);
}
} // namespace astro
