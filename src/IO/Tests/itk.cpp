#include <IO/itkinput.h>
#include <IO/itkoutput.h>

#include <itkImageRegionIterator.h>

#include <gtest/gtest.h>

TEST(ITK, Input)
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

TEST(ITK, Output)
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
