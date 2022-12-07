#pragma once

#include <IO/io.h>

#include <H5Cpp.h>

namespace astro
{
namespace hdf5
{
ASTRO_IO_EXPORT H5::Group getGroupForDataset(const H5::H5File& dataset, const std::string& datasetName);
ASTRO_IO_EXPORT ImageTypePtr extractFrom(const H5::DataSet& dataset);
ASTRO_IO_EXPORT ImageTypePtr extractFrom(const H5::DataSet& dataset, size_t index);
ASTRO_IO_EXPORT H5::DataSet writeTo(const ImageTypePtr& img, H5::Group& group, const std::string& datasetName);
ASTRO_IO_EXPORT H5::DataSet readTo(const std::vector<std::string>& filenames, itk::Size<Dimension> size,
                                   H5::Group& group, const std::string& datasetName);
} // namespace hdf5
} // namespace astro