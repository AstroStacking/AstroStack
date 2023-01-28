#include <Algos/ImageStats/center.h>

#include <gtest/gtest.h>

TEST(ImageStats_Center, simple)
{
    astro::image_statistics::Center center(2);

    center.process(1, 0, itk::Index<2>({1, 2}));
    center.process(.5, 1, itk::Index<2>({10, 20}));
    center.process(.1, 1, itk::Index<2>({100, 200}));

    center.compute();

    ASSERT_NEAR(center.getData(0, 0), 1, 0.00001);
    ASSERT_NEAR(center.getData(0, 1), 2, 0.00001);
    ASSERT_NEAR(center.getData(1, 0), 25, 0.00001);
    ASSERT_NEAR(center.getData(1, 1), 50, 0.00001);
}
