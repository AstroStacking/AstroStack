#pragma once


#include <IO/config.h>
#include <IO/io.h>

namespace astro
{
namespace io
{
template<typename OutputType>
ASTRO_IO_EXPORT void save(const ImageTypePtr& img, const std::string& filename);
template<typename OutputType>
ASTRO_IO_EXPORT void save(const ScalarImageTypePtr& img, const std::string& filename);
} // namespace io
} // namespace astro
