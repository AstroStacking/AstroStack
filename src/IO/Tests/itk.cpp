#include <IO/itkinput.h>
#include <IO/itkoutput.h>

#include <itkImageRegionIterator.h>

#include <gtest/gtest.h>

TEST(ITK, InputBlack)
{
    auto img = astro::io::open("black.png");
    itk::Size<astro::Dimension> size{20, 10};
    ASSERT_EQ(img->GetRequestedRegion().GetSize(), size);

    astro::PixelType black(0.f);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        ASSERT_EQ(value, black);
        ++it;
    }
}

TEST(ITK, OutputBlack)
{
    auto img = astro::io::open("black.png");
    astro::io::save<uint8_t>(img, "black_bis.png");

    img = astro::io::open("black_bis.png");
    itk::Size<astro::Dimension> size{20, 10};
    ASSERT_EQ(img->GetRequestedRegion().GetSize(), size);

    astro::PixelType black(0.f);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        ASSERT_EQ(value, black);
        ++it;
    }
}

TEST(ITK, InputGradient)
{
    auto img = astro::io::open("gradient.png");
    itk::Size<astro::Dimension> size{256, 3};
    ASSERT_EQ(img->GetRequestedRegion().GetSize(), size);

    astro::PixelType black(0.f);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();
    int pixel = 0;

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        int i = pixel / 256;
        int j = pixel % 256;
        for (int k = 0; k < 3; ++k)
        {
            ASSERT_NEAR(value[k], k == i ? j / 255.f : 0, 0.00001);
        }
        ++it;
        ++pixel;
    }
}

TEST(ITK, InputGradientGreyed)
{
    auto img = astro::io::open("gradient_greyed.png");
    itk::Size<astro::Dimension> size{256, 3};
    ASSERT_EQ(img->GetRequestedRegion().GetSize(), size);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();
    int pixel = 0;

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        int i = pixel / 256;
        int j = pixel % 256;
        for (int k = 0; k < 3; ++k)
        {
            ASSERT_NEAR(value[k], j / 255.f, 0.00001);
        }
        ++it;
        ++pixel;
    }
}
