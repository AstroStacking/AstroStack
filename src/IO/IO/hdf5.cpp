#include "hdf5.h"

#include <IO/itkinput.h>

#include <itkImportImageFilter.h>

namespace astro
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
    ndims = dataspace.getSimpleExtentDims(dims, NULL);
    if (dims[2] != 3)
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
    return importFilter->GetOutput();
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
        hsize_t offset[4] = {i, 0, 0, 0};
        hsize_t currSlab[4]{static_cast<hsize_t>(i), size.at(0), size.at(1), PixelDimension};
        fspace1.selectHyperslab(H5S_SELECT_SET, currSlab, offset);

        auto img = io::open(filenames[i]);

        dataset.write(img.GetPointer(), H5::PredType::NATIVE_FLOAT, dataspace, fspace1);
    }

    return dataset;
}
} // namespace astro
