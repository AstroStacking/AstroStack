#include <Algos/Filters/Stackers/robustmean.h>

#include <gtest/gtest.h>

TEST(RobustMean, simple)
{
    std::vector<float> data{0, 1, 2, 3, 4, -100, 5, 6, 7, 8, 9};
    ASSERT_EQ(astro::filters::stackers::RobustMean<float>(.1)(data), 4);
}

TEST(RobustMean, nanSimple)
{
    std::vector<float> data{std::numeric_limits<float>::quiet_NaN(), 0, 1, 2, 3, 4, -100, 5, 6, 7, 8, 9, std::numeric_limits<float>::quiet_NaN()};
    ASSERT_EQ(astro::filters::stackers::RobustMean<float>(.1)(data), 4);
}
