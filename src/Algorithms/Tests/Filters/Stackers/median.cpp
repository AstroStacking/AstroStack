#include <Algos/Filters/Stackers/median.h>

#include <gtest/gtest.h>

TEST(Median, simple)
{
    std::vector<float> data{0, 1, 2, 3, 4, -100, 5, 6, 7, 8, 9};
    ASSERT_EQ(astro::filters::stackers::Median<float>()(data), 4);
}
