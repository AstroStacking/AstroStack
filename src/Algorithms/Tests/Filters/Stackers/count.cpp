#include <Algos/Filters/Stackers/count.h>

#include <gtest/gtest.h>

TEST(Count, simple)
{
    std::vector<float> data{0.5, 1, 2, 3, 4, -100, 5, 6, 7, 8, 9};
    ASSERT_EQ(astro::filters::stackers::Count<float>()(data), 1);
}

TEST(Count, nanSimple)
{
    std::vector<float> data{0, 1, std::numeric_limits<float>::quiet_NaN(), 2, 3, 4, -100, 5, 6, 7, 8, 9};
    ASSERT_EQ(astro::filters::stackers::Count<float>()(data), 10. / 12);
}
