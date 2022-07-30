#include <Algos/Filters/Convertors/RGB2HSL.h>

#include <gtest/gtest.h>

TEST(RGB2HSL, black)
{
    itk::RGBPixel<double> rgb;

    auto hsl = astro::filters::convertors::RGB2HSL(rgb);
    
    ASSERT_EQ(hsl[0], 0);
    ASSERT_EQ(hsl[1], 0);
    ASSERT_EQ(hsl[2], 0);
}

TEST(RGB2HSL, white)
{
    itk::RGBPixel<double> rgb(1);

    auto hsl = astro::filters::convertors::RGB2HSL(rgb);
    
    ASSERT_EQ(hsl[0], 0);
    ASSERT_EQ(hsl[1], 0);
    ASSERT_EQ(hsl[2], 1);
}

TEST(RGB2HSL, red)
{
    itk::RGBPixel<double> rgb;
    rgb[0] = 1;

    auto hsl = astro::filters::convertors::RGB2HSL(rgb);
    
    ASSERT_EQ(hsl[0], 0);
    ASSERT_EQ(hsl[1], 1);
    ASSERT_EQ(hsl[2], .5);
}

TEST(RGB2HSL, blue)
{
    itk::RGBPixel<double> rgb;
    rgb[2] = 1;

    auto hsl = astro::filters::convertors::RGB2HSL(rgb);
    
    ASSERT_EQ(hsl[0], 240);
    ASSERT_EQ(hsl[1], 1);
    ASSERT_EQ(hsl[2], .5);
}

TEST(RGB2HSL, green)
{
    itk::RGBPixel<double> rgb;
    rgb[1] = 1;

    auto hsl = astro::filters::convertors::RGB2HSL(rgb);
    
    ASSERT_EQ(hsl[0], 120);
    ASSERT_EQ(hsl[1], 1);
    ASSERT_EQ(hsl[2], .5);
}
