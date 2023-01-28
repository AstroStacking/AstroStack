#include <Algos/ImageStats/mean.h>

#include <gtest/gtest.h>

TEST(ImageStats_Mean, build)
{
    astro::image_statistics::Mean mean(2);

    mean.process(1, 0, itk::Index<2>({1, 2}));
    mean.process(.5, 1, itk::Index<2>({10, 20}));
    mean.process(.1, 1, itk::Index<2>({100, 200}));

    mean.compute();

    ASSERT_NEAR(mean.getData(0, 0), 1, 0.00001);
    ASSERT_NEAR(mean.getData(1, 0), .3, 0.00001);
}
