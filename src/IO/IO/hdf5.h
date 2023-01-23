#pragma once

#include <IO/io.h>

#include <H5Cpp.h>

namespace astro
{
namespace hdf5
{
ASTRO_IO_EXPORT ImageTypePtr extractFrom(const H5::DataSet& dataset);
ASTRO_IO_EXPORT ImageTypePtr extractFrom(const H5::DataSet& dataset, size_t index);
ASTRO_IO_EXPORT ScalarImageTypePtr extractScalarFrom(const H5::DataSet& dataset);
ASTRO_IO_EXPORT H5::DataSet writeTo(const ImageType& img, const H5::Group& group, const std::string& datasetName);
ASTRO_IO_EXPORT H5::DataSet writeTo(const ScalarImageType& img, const H5::Group& group, const std::string& datasetName);
ASTRO_IO_EXPORT H5::DataSet readTo(const std::vector<std::string>& filenames, itk::Size<Dimension> size,
                                   const H5::Group& group, const std::string& datasetName);
ASTRO_IO_EXPORT H5::DataSet createDataset(const std::string& outputDatasetName, const H5::DataSpace& outputSpace,
                                          H5::PredType type, const H5::H5File& h5file);
} // namespace hdf5
} // namespace astro
