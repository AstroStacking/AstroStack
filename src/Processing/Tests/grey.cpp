#include <IO/hdf5.h>
#include <IO/tests/hdf5suite.h>
#include <Processing/grey.h>

#include <gtest/gtest.h>

#include <itkImageRegionIterator.h>

TEST_F(HDF5TestSuite, ProcessGrey)
{
    itk::Size<astro::Dimension> size{256, 3};
    H5::DataSet inputs = astro::hdf5::readTo({"gradient.png"}, size, file.createGroup("test"), "inputs");

    auto result = astro::processing::grey(inputs, 0, file.createGroup("output"), "grey");

    ASSERT_EQ(result->GetRequestedRegion().GetSize(), size);

    astro::PixelType black(0.f);

    using IteratorType = itk::ImageRegionIterator<astro::ScalarImageType>;
    IteratorType it(result, result->GetRequestedRegion());
    it.GoToBegin();

    int pixel = 0;
    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        int j = pixel % 256;
        ASSERT_NEAR(value, j / 255.f / 2, 0.0000001);
        ++it;
        ++pixel;
    }
}
