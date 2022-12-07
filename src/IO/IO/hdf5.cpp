#include "hdf5.h"

#include <IO/itkinput.h>

#include <itkCastImageFilter.h>
#include <itkImportImageFilter.h>

namespace astro
{
namespace hdf5
{
H5::Group getGroupForDataset(const H5::H5File& dataset, const std::string& datasetName)
{
    size_t needSubGroup = datasetName.rfind("/");
    if (needSubGroup == std::string::npos)
    {
        return dataset;
    }
    try
    {
        return dataset.openGroup(datasetName.substr(0, needSubGroup - 1));
    }
    catch (const std::exception&)
    {
        return dataset.createGroup(datasetName.substr(0, needSubGroup - 1));
    }
}

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
    std::vector<astro::PixelType> buffer(dims[0] * dims[1]);
    dataset.read(buffer.data(), H5::PredType::NATIVE_FLOAT, dataspace, dataspace);

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

    using ImportFilterType = itk::ImportImageFilter<astro::PixelType, astro::Dimension>;
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
    std::vector<astro::PixelType> buffer(dims[0] * dims[1]);

    hsize_t inputDims[4]{1, dims[1], dims[2], PixelDimension};
    H5::DataSpace memoryDataspace(4, inputDims);

    H5::DataSpace fspace1 = dataset.getSpace();
    hsize_t currSlab[4]{1, dims[1], dims[2], PixelDimension};
    hsize_t offset[4]{index, 0, 0, 0};
    fspace1.selectHyperslab(H5S_SELECT_SET, currSlab, offset);

    dataset.read(buffer.data(), H5::PredType::NATIVE_FLOAT, memoryDataspace, fspace1);

    importFilter->SetImportPointer(buffer.data(), buffer.size(), importImageFilterWillOwnTheBuffer);
    importFilter->Update();
    return importFilter->GetOutput();
}

H5::DataSet writeTo(const ImageTypePtr& img, H5::Group& group, const std::string& datasetName)
{
    itk::Size<Dimension> size = img->GetRequestedRegion().GetSize();
    hsize_t maxDims[3]{size.at(0), size.at(1), PixelDimension};
    hsize_t inputDim[3]{size.at(0), size.at(1), PixelDimension};
    H5::DataSpace dataspace(3, inputDim);
    H5::DataSet dataset = group.createDataSet(datasetName, H5::PredType::NATIVE_FLOAT, dataspace);
    dataset.write(img->GetBufferPointer(), H5::PredType::NATIVE_FLOAT, dataspace, dataspace);
    return dataset;
}

H5::DataSet readTo(const std::vector<std::string>& filenames, itk::Size<Dimension> size, H5::Group& group,
                   const std::string& datasetName)
{
    hsize_t maxDims[4]{static_cast<hsize_t>(filenames.size()), size.at(0), size.at(1), PixelDimension};
    hsize_t inputDim[4]{1, size.at(0), size.at(1), PixelDimension};
    H5::DataSpace dataspace(4, inputDim);
    H5::DataSet dataset = group.createDataSet(datasetName, H5::PredType::NATIVE_FLOAT, dataspace);

    for (size_t i = 0; i < filenames.size(); ++i)
    {
        H5::DataSpace fspace1 = dataset.getSpace();
        hsize_t currSlab[4]{1, size.at(0), size.at(1), PixelDimension};
        hsize_t offset[4]{i, 0, 0, 0};
        fspace1.selectHyperslab(H5S_SELECT_SET, currSlab, offset);

        auto img = io::open(filenames[i]);

        dataset.write(img->GetBufferPointer(), H5::PredType::NATIVE_FLOAT, dataspace, fspace1);
    }

    return dataset;
}
} // namespace hdf5
} // namespace astro