#include "hdf5.h"

#include <IO/itkinput.h>
#include <IO/traits.h>

#include <itkCastImageFilter.h>
#include <itkImportImageFilter.h>

#ifdef ASTROSTACK_TBB
#include <tbb/tbb.h>
#endif

namespace astro
{
namespace hdf5
{
ImageTypePtr extractFrom(const H5::DataSet& dataset)
{
    H5::DataSpace dataspace = dataset.getSpace();
    int ndims = dataspace.getSimpleExtentNdims();
    if (ndims != 3)
    {
        throw std::runtime_error("Wrong number of dimensions");
    }
    hsize_t dims[3];
    ndims = dataspace.getSimpleExtentDims(dims, nullptr);
    if (dims[2] != PixelDimension)
    {
        throw std::runtime_error("Wrong pixel dimension");
    }

    std::vector<astro::PixelType> buffer(dims[0] * dims[1]);
    dataset.read(buffer.data(), H5::PredType::NATIVE_FLOAT, dataspace, dataspace);

    using ImportFilterType = itk::ImportImageFilter<astro::PixelType, astro::Dimension>;
    auto importFilter = ImportFilterType::New();
    ImportFilterType::SizeType regionSize;

    regionSize[0] = dims[0]; // size along X
    regionSize[1] = dims[1]; // size along Y

    ImportFilterType::IndexType start;
    start.Fill(0);

    ImportFilterType::RegionType region;
    region.SetIndex(start);
    region.SetSize(regionSize);
    importFilter->SetRegion(region);

    const itk::SpacePrecisionType origin[astro::Dimension] = {0.0, 0.0};
    importFilter->SetOrigin(origin);
    const itk::SpacePrecisionType spacing[astro::Dimension] = {1.0, 1.0};
    importFilter->SetSpacing(spacing);

    const bool importImageFilterWillOwnTheBuffer = false;

    importFilter->SetImportPointer(buffer.data(), buffer.size(), importImageFilterWillOwnTheBuffer);
    importFilter->Update();

    using CastFilterType = itk::CastImageFilter<ImageType, ImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(importFilter->GetOutput());
    castFilter->Update();
    return castFilter->GetOutput();
}

ASTRO_IO_EXPORT ImageTypePtr extractFrom(const H5::DataSet& dataset, size_t index)
{
    H5::DataSpace dataspace = dataset.getSpace();
    int ndims = dataspace.getSimpleExtentNdims();
    if (ndims != 4)
    {
        throw std::runtime_error("Wrong number of dimensions");
    }
    hsize_t dims[4];
    ndims = dataspace.getSimpleExtentDims(dims, nullptr);
    if (dims[3] != PixelDimension)
    {
        throw std::runtime_error("Wrong pixel dimension");
    }

    hsize_t inputDims[4]{1, dims[1], dims[2], Traits<ImageType>::LastDim};
    H5::DataSpace memoryDataspace(4, inputDims);

    H5::DataSpace fspace1 = dataset.getSpace();
    hsize_t offset[4]{index, 0, 0, 0};
    fspace1.selectHyperslab(H5S_SELECT_SET, inputDims, offset);

    std::vector<astro::PixelType> buffer(dims[1] * dims[2]);
    dataset.read(buffer.data(), H5::PredType::NATIVE_FLOAT, memoryDataspace, fspace1);

    using ImportFilterType = itk::ImportImageFilter<Traits<ImageType>::PixelType, astro::Dimension>;
    auto importFilter = ImportFilterType::New();
    ImportFilterType::SizeType regionSize;

    regionSize[0] = dims[1]; // size along X
    regionSize[1] = dims[2]; // size along Y

    ImportFilterType::IndexType start;
    start.Fill(0);

    ImportFilterType::RegionType region;
    region.SetIndex(start);
    region.SetSize(regionSize);
    importFilter->SetRegion(region);

    const itk::SpacePrecisionType origin[astro::Dimension] = {0.0, 0.0};
    importFilter->SetOrigin(origin);
    const itk::SpacePrecisionType spacing[astro::Dimension] = {1.0, 1.0};
    importFilter->SetSpacing(spacing);

    const bool importImageFilterWillOwnTheBuffer = false;
    importFilter->SetImportPointer(buffer.data(), buffer.size(), importImageFilterWillOwnTheBuffer);
    importFilter->Update();

    using CastFilterType = itk::CastImageFilter<ImageType, ImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(importFilter->GetOutput());
    castFilter->Update();
    return castFilter->GetOutput();
}

ScalarImageTypePtr extractScalarFrom(const H5::DataSet& dataset)
{
    H5::DataSpace dataspace = dataset.getSpace();
    int ndims = dataspace.getSimpleExtentNdims();
    if (ndims != 2)
    {
        throw std::runtime_error("Wrong number of dimensions");
    }
    hsize_t dims[2];
    ndims = dataspace.getSimpleExtentDims(dims, nullptr);

    std::vector<astro::UnderlyingPixelType> buffer(dims[0] * dims[1]);
    dataset.read(buffer.data(), H5::PredType::NATIVE_FLOAT, dataspace, dataspace);

    using ImportFilterType = itk::ImportImageFilter<astro::UnderlyingPixelType, astro::Dimension>;
    auto importFilter = ImportFilterType::New();
    ImportFilterType::SizeType regionSize;

    regionSize[0] = dims[0]; // size along X
    regionSize[1] = dims[1]; // size along Y

    ImportFilterType::IndexType start;
    start.Fill(0);

    ImportFilterType::RegionType region;
    region.SetIndex(start);
    region.SetSize(regionSize);
    importFilter->SetRegion(region);

    const itk::SpacePrecisionType origin[astro::Dimension] = {0.0, 0.0};
    importFilter->SetOrigin(origin);
    const itk::SpacePrecisionType spacing[astro::Dimension] = {1.0, 1.0};
    importFilter->SetSpacing(spacing);

    const bool importImageFilterWillOwnTheBuffer = false;

    importFilter->SetImportPointer(buffer.data(), buffer.size(), importImageFilterWillOwnTheBuffer);
    importFilter->Update();

    using CastFilterType = itk::CastImageFilter<ScalarImageType, ScalarImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(importFilter->GetOutput());
    castFilter->Update();
    return castFilter->GetOutput();
}

ScalarImageTypePtr extractScalarFrom(const H5::DataSet& dataset, size_t index)
{
    H5::DataSpace dataspace = dataset.getSpace();
    int ndims = dataspace.getSimpleExtentNdims();
    if (ndims != 3)
    {
        throw std::runtime_error("Wrong number of dimensions");
    }
    hsize_t dims[3];
    ndims = dataspace.getSimpleExtentDims(dims, nullptr);

    hsize_t inputDims[3]{1, dims[1], dims[2]};
    H5::DataSpace memoryDataspace(3, inputDims);

    H5::DataSpace fspace1 = dataset.getSpace();
    hsize_t offset[3]{index, 0, 0};
    fspace1.selectHyperslab(H5S_SELECT_SET, inputDims, offset);

    std::vector<astro::UnderlyingPixelType> buffer(dims[1] * dims[2]);
    dataset.read(buffer.data(), H5::PredType::NATIVE_FLOAT, memoryDataspace, fspace1);

    using ImportFilterType = itk::ImportImageFilter<astro::UnderlyingPixelType, astro::Dimension>;
    auto importFilter = ImportFilterType::New();
    ImportFilterType::SizeType regionSize;

    regionSize[0] = dims[1]; // size along X
    regionSize[1] = dims[2]; // size along Y

    ImportFilterType::IndexType start;
    start.Fill(0);

    ImportFilterType::RegionType region;
    region.SetIndex(start);
    region.SetSize(regionSize);
    importFilter->SetRegion(region);

    const itk::SpacePrecisionType origin[astro::Dimension] = {0.0, 0.0};
    importFilter->SetOrigin(origin);
    const itk::SpacePrecisionType spacing[astro::Dimension] = {1.0, 1.0};
    importFilter->SetSpacing(spacing);

    const bool importImageFilterWillOwnTheBuffer = false;
    importFilter->SetImportPointer(buffer.data(), buffer.size(), importImageFilterWillOwnTheBuffer);
    importFilter->Update();

    using CastFilterType = itk::CastImageFilter<ScalarImageType, ScalarImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(importFilter->GetOutput());
    castFilter->Update();
    return castFilter->GetOutput();
}

H5::DataSet writeTo(const ImageType& img, const H5::Group& group, const std::string& datasetName)
{
    itk::Size<Dimension> size = img.GetRequestedRegion().GetSize();
    hsize_t inputDim[3]{size.at(0), size.at(1), Traits<ImageType>::LastDim};
    H5::DataSpace dataspace(3, inputDim);
    H5::DataSet dataset = createDataset<float>(datasetName, dataspace, group);
    dataset.write(img.GetBufferPointer(), H5::PredType::NATIVE_FLOAT, dataspace, dataspace);
    return dataset;
}

H5::DataSet writeTo(const ScalarImageType& img, const H5::Group& group, const std::string& datasetName)
{
    itk::Size<Dimension> size = img.GetRequestedRegion().GetSize();
    hsize_t inputDim[2]{size.at(0), size.at(1)};
    H5::DataSpace dataspace(2, inputDim);
    H5::DataSet dataset = createDataset<float>(datasetName, dataspace, group);
    dataset.write(img.GetBufferPointer(), H5::PredType::NATIVE_FLOAT, dataspace, dataspace);
    return dataset;
}

void writeTo(const ImageType& img, const H5::DataSet& dataset, size_t index)
{
    auto size = img.GetRequestedRegion().GetSize();
    hsize_t imageDim[4]{1, size.at(0), size.at(1), PixelDimension};
    H5::DataSpace imgDataspace(4, imageDim);

    H5::DataSpace fspace1 = dataset.getSpace();
    hsize_t currSlab[4]{1, size.at(0), size.at(1), PixelDimension};
    hsize_t offset[4]{index, 0, 0, 0};
    fspace1.selectHyperslab(H5S_SELECT_SET, currSlab, offset);

    dataset.write(img.GetBufferPointer(), H5::PredType::NATIVE_FLOAT, imgDataspace, fspace1);
}

void writeTo(const ScalarImageType& img, const H5::DataSet& dataset, size_t index)
{
    auto size = img.GetRequestedRegion().GetSize();
    hsize_t imageDim[3]{1, size.at(0), size.at(1)};
    H5::DataSpace imgDataspace(3, imageDim);

    H5::DataSpace fspace1 = dataset.getSpace();
    hsize_t currSlab[3]{1, size.at(0), size.at(1)};
    hsize_t offset[3]{index, 0, 0};
    fspace1.selectHyperslab(H5S_SELECT_SET, currSlab, offset);

    dataset.write(img.GetBufferPointer(), H5::PredType::NATIVE_FLOAT, imgDataspace, fspace1);
}

H5::DataSet readTo(const std::vector<std::string>& filenames, itk::Size<Dimension> size, const H5::Group& group,
                   const std::string& datasetName, std::optional<std::function<void()>> updateTask)
{
    hsize_t imageDim[4]{1, size.at(0), size.at(1), PixelDimension};
    H5::DataSpace imgDataspace(4, imageDim);

    hsize_t inputDim[4]{filenames.size(), size.at(0), size.at(1), PixelDimension};
    H5::DataSpace dataspace(4, inputDim);
    H5::DataSet dataset = createDataset<float>(datasetName, dataspace, group);

    std::mutex mutex;

#ifdef ASTROSTACK_TBB
    tbb::parallel_for(size_t(0), filenames.size(), [&](size_t index)
#else
    for (size_t index = 0; index < filenames.size(); ++index)
#endif
    {

        auto img = io::open(filenames[index]);
        if (!img)
        {
            throw std::runtime_error("Failed to read " + filenames[index]);
        }

        {
            std::lock_guard<std::mutex> lock(mutex);
            H5::DataSpace fspace1 = dataset.getSpace();
            hsize_t currSlab[4]{1, size.at(0), size.at(1), PixelDimension};
            hsize_t offset[4]{index, 0, 0, 0};
            fspace1.selectHyperslab(H5S_SELECT_SET, currSlab, offset);
            dataset.write(img->GetBufferPointer(), H5::PredType::NATIVE_FLOAT, imgDataspace, fspace1);
        }
        if (updateTask)
        {
            (*updateTask)();
        }
    }
#ifdef ASTROSTACK_TBB
    );
#endif

    return dataset;
}

H5::Group getOrCreateGroup(const std::string& outputGroupName, const H5::Group& h5file)
{
    if (outputGroupName.empty())
    {
        return h5file;
    }
    try
    {
        return h5file.openGroup(outputGroupName);
    }
    catch (const H5::FileIException&)
    {
        return h5file.createGroup(outputGroupName);
    }
    catch (const H5::GroupIException&)
    {
        return h5file.createGroup(outputGroupName);
    }
}

template<typename T>
H5::DataSet createDataset(const std::string& outputDatasetName, const H5::DataSpace& outputSpace,
                          const H5::Group& h5file)
{
    H5::DataSet outputDataset;
    H5::DSetCreatPropList plist;
    plist.setFillValue(DataTraits<T>::HDF5Type, &DataTraits<T>::defaultValue);

    size_t needSubGroup = outputDatasetName.rfind("/");
    if (needSubGroup == std::string::npos)
    {
        outputDataset = h5file.createDataSet(outputDatasetName, DataTraits<T>::HDF5Type, outputSpace);
    }
    else
    {
        H5::Group group = getOrCreateGroup(outputDatasetName.substr(0, needSubGroup), h5file);
        outputDataset =
                group.createDataSet(outputDatasetName.substr(needSubGroup + 1), DataTraits<T>::HDF5Type, outputSpace);
    }
    return outputDataset;
}

template ASTRO_IO_EXPORT H5::DataSet createDataset<double>(const std::string& outputDatasetName,
                                                           const H5::DataSpace& outputSpace, const H5::Group& h5file);
template ASTRO_IO_EXPORT H5::DataSet createDataset<double>(const std::string& outputDatasetName,
                                                           const H5::DataSpace& outputSpace, const H5::Group& h5file);
template ASTRO_IO_EXPORT H5::DataSet createDataset<uint64_t>(const std::string& outputDatasetName,
                                                             const H5::DataSpace& outputSpace, const H5::Group& h5file);

template<typename T>
std::vector<std::pair<T, T>> readGraph(const H5::DataSet& dataset)
{
    H5::DataSpace dataspace = dataset.getSpace();
    int ndims = dataspace.getSimpleExtentNdims();
    if (ndims != 2)
    {
        throw std::runtime_error("Wrong number of dimensions");
    }
    hsize_t dims[2];
    ndims = dataspace.getSimpleExtentDims(dims, nullptr);
    if (dims[1] < 2)
    {
        throw std::runtime_error("Wrong number of columns " + std::to_string(dims[1]));
    }

    std::vector<T> buffer(dims[0] * dims[1]);
    dataset.read(buffer.data(), DataTraits<T>::HDF5Type, dataspace, dataspace);

    std::vector<std::pair<T, T>> data(dims[0]);
    for (size_t i = 0; i < dims[0]; ++i)
    {
        data[i].first = buffer[i * dims[1]];
        data[i].second = buffer[i * dims[1] + 1];
    }

    return data;
}

template ASTRO_IO_EXPORT std::vector<std::pair<double, double>> readGraph<double>(const H5::DataSet& dataset);
template ASTRO_IO_EXPORT std::vector<std::pair<uint64_t, uint64_t>> readGraph<uint64_t>(const H5::DataSet& dataset);

template<typename T>
void writeGraph(const std::vector<std::pair<T, T>>& graph, const H5::Group& group, const std::string& datasetName)
{
    // Output data
    hsize_t outputImgDim[2]{graph.size() / 2, 2};
    H5::DataSpace outputSpace(2, outputImgDim);
    // output list
    H5::DataSet outputDataset = createDataset<T>(datasetName, outputSpace, group);
    outputDataset.write(graph.data(), DataTraits<T>::HDF5Type, outputSpace, outputSpace);
}

template ASTRO_IO_EXPORT void writeGraph<double>(const std::vector<std::pair<double, double>>& graph,
                                                const H5::Group& h5file,
                                 const std::string& datasetName);
template ASTRO_IO_EXPORT void writeGraph<uint64_t>(const std::vector<std::pair<uint64_t, uint64_t>>& graph,
                                                   const H5::Group& h5file,
                          const std::string& datasetName);

} // namespace hdf5
} // namespace astro
