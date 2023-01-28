#include <Algos/ImageStats/minmax_position.h>

#include <gtest/gtest.h>

TEST(ImageStats_MinMax, build)
{
    astro::image_statistics::MinMaxPosition minmaxposition(2);

    minmaxposition.process(1, 0, itk::Index<2>({1, 2}));
    minmaxposition.process(1, 0, itk::Index<2>({10, 20}));
    minmaxposition.process(1, 1, itk::Index<2>({10, 20}));
    minmaxposition.process(1, 1, itk::Index<2>({100, 200}));
    minmaxposition.compute();

    ASSERT_NEAR(minmaxposition.getData(0, 0), 1, 0.00001);
    ASSERT_NEAR(minmaxposition.getData(0, 1), 10, 0.00001);
    ASSERT_NEAR(minmaxposition.getData(0, 2), 2, 0.00001);
    ASSERT_NEAR(minmaxposition.getData(0, 3), 20, 0.00001);
    ASSERT_NEAR(minmaxposition.getData(1, 0), 10, 0.00001);
    ASSERT_NEAR(minmaxposition.getData(1, 1), 100, 0.00001);
    ASSERT_NEAR(minmaxposition.getData(1, 2), 20, 0.00001);
    ASSERT_NEAR(minmaxposition.getData(1, 3), 200, 0.00001);
}
