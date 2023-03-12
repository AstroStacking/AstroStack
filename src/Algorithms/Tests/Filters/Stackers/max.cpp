#include <Algos/Filters/Stackers/max.h>

#include <gtest/gtest.h>

TEST(Max, simple)
{
    std::vector<float> data{0, 1, 2, 3, 4, -100, 5, 6, 7, 8, 9};
    ASSERT_EQ(astro::filters::stackers::Max<float>()(data), 9);
}

TEST(Max, nanSimple)
{
    std::vector<float> data{0, 1, std::numeric_limits<float>::quiet_NaN(), 2, 3, 4, -100, 5, 6, 7, 8, 9};
    ASSERT_EQ(astro::filters::stackers::Max<float>()(data), 9);
}
