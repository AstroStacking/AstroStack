#include <Algos/ImageStats/size.h>

#include <gtest/gtest.h>

TEST(ImageStats_Size, build)
{
    astro::image_statistics::Size size(2);

    size.process(1, 0, itk::Index<2>({1, 2}));
    size.process(2, 0, itk::Index<2>({1, 2}));
    size.process(0.5, 1, itk::Index<2>({1, 2}));
    size.process(5, 1, itk::Index<2>({1, 2}));
    size.compute();

    ASSERT_NEAR(size.getData(0, 0), 3, 0.00001);
    ASSERT_NEAR(size.getData(1, 0), 5.5, 0.00001);
}
