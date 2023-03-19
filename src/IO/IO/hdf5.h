#pragma once

#include <IO/io.h>

#include <H5Cpp.h>

#include <optional>

namespace astro
{
namespace hdf5
{
ASTRO_IO_EXPORT ImageTypePtr extractFrom(const H5::DataSet& dataset);
ASTRO_IO_EXPORT ImageTypePtr extractFrom(const H5::DataSet& dataset, size_t index);
ASTRO_IO_EXPORT ScalarImageTypePtr extractScalarFrom(const H5::DataSet& dataset);
ASTRO_IO_EXPORT ScalarImageTypePtr extractScalarFrom(const H5::DataSet& dataset, size_t index);
ASTRO_IO_EXPORT H5::DataSet writeTo(const ImageType& img, const H5::Group& group, const std::string& datasetName);
ASTRO_IO_EXPORT H5::DataSet writeTo(const ScalarImageType& img, const H5::Group& group, const std::string& datasetName);
ASTRO_IO_EXPORT void writeTo(const ImageType& img, const H5::DataSet& dataset, size_t index);
ASTRO_IO_EXPORT void writeTo(const ScalarImageType& img, const H5::DataSet& dataset, size_t index);
ASTRO_IO_EXPORT H5::DataSet
readTo(const std::vector<std::string>& filenames, itk::Size<Dimension> size, const H5::Group& group,
       const std::string& datasetName,
       std::optional<std::function<void()>> updateTask = std::optional<std::function<void()>>());
ASTRO_IO_EXPORT H5::Group getOrCreateGroup(const std::string& outputGroupName, const H5::Group& h5file);
template<typename T>
ASTRO_IO_EXPORT H5::DataSet createDataset(const std::string& outputDatasetName, const H5::DataSpace& outputSpace,
                                          const H5::Group& h5file);
template<typename T>
ASTRO_IO_EXPORT std::vector<std::pair<T, T>> readGraph(const H5::DataSet& dataset);
template<typename T>
ASTRO_IO_EXPORT void writeGraph(const std::vector<std::pair<T, T>>& graph, const H5::Group& h5file,
                                const std::string& datasetName);

} // namespace hdf5
} // namespace astro
