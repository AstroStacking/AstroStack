#include <IO/itkinput.h>
#include <Processing/chromasmoothing.h>
#include <Algos/Filters/Convertors/RGB2HSL.h>

#include <gtest/gtest.h>

#include <itkImageRegionIterator.h>

TEST(Processing, ChromaSmoothing)
{
    auto img = astro::io::open("rgb.png");

    constexpr double variance = 5;
    
    auto result = astro::processing::chromaSmoothing(img, variance);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(result, result->GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        ASSERT_NEAR(astro::filters::convertors::RGB2L(it.Get()), 0.5, 0.000001);
        ++it;
    }
}
