#include <Algos/Filters/Stackers/robustmean.h>

#include <gtest/gtest.h>

TEST(RobustMean, simple)
{
    std::vector<float> data{-100, 0, 1, 2, 3, 4, 5};
    ASSERT_EQ(astro::filters::stackers::RobustMean<float>()(data), 2.5);
}
