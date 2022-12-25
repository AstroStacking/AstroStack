#include <IO/itkinput.h>
#include <Processing/exponential.h>

#include <gtest/gtest.h>

#include <itkImageRegionIterator.h>

TEST(Processing, Exponential)
{
    auto img = astro::io::open("gradient.png");

    constexpr double exponent = 10;
    
    auto result = astro::processing::exponential(img, exponent);

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
            ASSERT_NEAR(value[k], k == i ? std::pow(j / 255.f, exponent) : 0, 0.00001);
        }
        ++it;
        ++pixel;
    }
}
