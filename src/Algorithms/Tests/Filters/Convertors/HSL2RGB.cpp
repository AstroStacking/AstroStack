#include <Algos/Filters/Convertors/HSL2RGB.h>

#include <gtest/gtest.h>

TEST(HSL2RGB, black)
{
    itk::RGBPixel<double> hsl;

    auto rgb = astro::filters::convertors::HSL2RGB(hsl);
    
    ASSERT_EQ(rgb[0], 0);
    ASSERT_EQ(rgb[1], 0);
    ASSERT_EQ(rgb[2], 0);
}

TEST(HSL2RGB, white)
{
    itk::RGBPixel<double> hsl;
    hsl[2] = 1;

    auto rgb = astro::filters::convertors::HSL2RGB(hsl);
    
    ASSERT_EQ(rgb[0], 1);
    ASSERT_EQ(rgb[1], 1);
    ASSERT_EQ(rgb[2], 1);
}
