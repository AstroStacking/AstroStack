#include <IO/itkinput.h>
#include <Processing/histostretch.h>

#include <gtest/gtest.h>

#include <itkImageRegionIterator.h>

TEST(Processing, HistoStretchAbsolute)
{
    auto img = astro::io::open("gradient.png");

    constexpr double limit = 0.1;

    auto result = astro::processing::histoStretch(img, limit, limit, limit, false);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(result, result->GetRequestedRegion());
    it.GoToBegin();

    int pixel = 0;
    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        int i = pixel / 256;
        int j = pixel % 256;
        for (int k = 0; k < 3; ++k)
        {
            ASSERT_NEAR(value[k], k == i ? std::max((j / 255.f - 0.1f) / 0.9f, 0.f) : 0, 0.0001);
        }
        ++it;
        ++pixel;
    }
}

TEST(Processing, HistoStretchRelative)
{
    auto img = astro::io::open("gradient.png");

    constexpr float limit = 0.1;

    auto result = astro::processing::histoStretch(img, limit, limit, limit, true);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(result, result->GetRequestedRegion());
    it.GoToBegin();

    int pixel = 0;
    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        int i = pixel / 256;
        int j = pixel % 256;
        for (int k = 0; k < 3; ++k)
        {
            ASSERT_NEAR(value[k], k == i ? std::max((j / 255.f - limit / 5) / (1 - limit / 5), 0.f) : 0, 0.1);
        }
        ++it;
        ++pixel;
    }
}
