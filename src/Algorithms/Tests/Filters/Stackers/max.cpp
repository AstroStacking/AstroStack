#include <Algos/Filters/Stackers/max.h>

#include <gtest/gtest.h>

TEST(Max, simple)
{
    std::vector<float> data{-100, 0, 1, 2, 3, 4, 5};
    ASSERT_EQ(astro::filters::stackers::Max<float>()(data), 5);
}
