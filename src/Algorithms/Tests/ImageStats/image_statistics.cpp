#include <Algos/ImageStats/center.h>
#include <Algos/ImageStats/image_statistics.h>
#include <Algos/ImageStats/mean.h>
#include <Algos/ImageStats/minmax_position.h>
#include <Algos/ImageStats/size.h>

#include <gtest/gtest.h>

TEST(ImageStats, build)
{
    using Stats =
            astro::image_statistics::ImageStatistics<astro::image_statistics::Center, astro::image_statistics::Mean,
                                                     astro::image_statistics::MinMaxPosition,
                                                     astro::image_statistics::Size>;
    Stats stats(2);

    stats.process(1, 1, itk::Index<2>({1, 2}));
    stats.process(1, 1, itk::Index<2>({10, 20}));
    stats.process(.5, 2, itk::Index<2>({10, 20}));
    stats.process(.1, 2, itk::Index<2>({100, 200}));
    stats.compute();

    ASSERT_NEAR(stats.getData()[0][0], 5.5, 0.00001);
    ASSERT_NEAR(stats.getData()[0][1], 11, 0.00001);
    ASSERT_NEAR(stats.getData()[0][2], 1, 0.00001);
    ASSERT_NEAR(stats.getData()[0][3], 1, 0.00001);
    ASSERT_NEAR(stats.getData()[0][4], 10, 0.00001);
    ASSERT_NEAR(stats.getData()[0][5], 2, 0.00001);
    ASSERT_NEAR(stats.getData()[0][6], 20, 0.00001);
    ASSERT_NEAR(stats.getData()[0][7], 2, 0.00001);

    ASSERT_NEAR(stats.getData()[1][0], 25, 0.00001);
    ASSERT_NEAR(stats.getData()[1][1], 50, 0.00001);
    ASSERT_NEAR(stats.getData()[1][2], .3, 0.00001);
    ASSERT_NEAR(stats.getData()[1][3], 10, 0.00001);
    ASSERT_NEAR(stats.getData()[1][4], 100, 0.00001);
    ASSERT_NEAR(stats.getData()[1][5], 20, 0.00001);
    ASSERT_NEAR(stats.getData()[1][6], 200, 0.00001);
    ASSERT_NEAR(stats.getData()[1][7], 2, 0.00001);
}
