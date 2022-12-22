#include <IO/hdf5.h>
#include <IO/tests/hdf5suite.h>

#include <itkImageRegionIterator.h>

TEST_F(HDF5TestSuite, ReadOneBlack)
{
    itk::Size<astro::Dimension> size{20, 10};
    H5::DataSet inputs = astro::hdf5::readTo({"black.png"}, size, file.createGroup("test"), "inputs");

    H5::DataSpace dataspace = inputs.getSpace();
    int ndims = dataspace.getSimpleExtentNdims();
    ASSERT_EQ(ndims, 4);
    hsize_t dims[4];
    ndims = dataspace.getSimpleExtentDims(dims, nullptr);
    ASSERT_EQ(dims[0], 1);
    ASSERT_EQ(dims[3], astro::PixelDimension);

    auto img = astro::hdf5::extractFrom(inputs, 0);

    ASSERT_EQ(img->GetRequestedRegion().GetSize(), size);

    astro::PixelType black(0.f);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        ASSERT_EQ(value, black);
        ++it;
    }
}
