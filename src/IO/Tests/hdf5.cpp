#include <IO/tests/hdf5suite.h>
#include <IO/hdf5.h>

#include <itkImageRegionIterator.h>

TEST_F(HDF5TestSuite, ReadOneBlack)
{
    itk::Size<astro::Dimension> size{20, 10};
    H5::DataSet inputs = astro::hdf5::readTo({"black.png"}, size, file.createGroup("test"), "inputs");
    
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
