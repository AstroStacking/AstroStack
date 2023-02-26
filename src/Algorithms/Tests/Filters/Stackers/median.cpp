#include <Algos/Filters/Stackers/median.h>

#include <gtest/gtest.h>

TEST(Median, simple)
{
    std::vector<float> data{-100, 0, 1, 2, 3, 4, 5};
    ASSERT_EQ(astro::filters::stackers::Median<float>()(data), 2);
}
