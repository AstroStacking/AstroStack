#pragma once

#include <IO/io.h>

#include <H5Cpp.h>

namespace astro
{
ASTRO_IO_EXPORT ImageTypePtr extractFrom(const H5::DataSet& dataset);
ASTRO_IO_EXPORT H5::DataSet readTo(const std::vector<std::string>& filenames, itk::Size<Dimension> size,
                                   H5::Group& group, const std::string& datasetName);
} // namespace astro
