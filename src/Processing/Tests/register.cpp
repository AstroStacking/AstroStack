#include <IO/itkinput.h>
#include <Processing/register.h>

#include <gtest/gtest.h>

#include <itkImageRegionIterator.h>

TEST(Processing, RegisterSquare)
{
    auto target = astro::io::open("square1.png");
    auto reference = astro::io::open("square2.png");

    constexpr double limit = 0.1;

    auto result = astro::processing::registerImages(
            reference, target, {{0, 0}, {0, 10}, {0, 20}, {10, 0}, {10, 10}, {10, 20}, {20, 0}, {20, 10}, {20, 20}},
            {{5, 10}, {5, 20}, {5, 30}, {15, 10}, {15, 20}, {15, 30}, {25, 10}, {25, 20}, {25, 30}}, 0);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(result, result->GetRequestedRegion());
    it.GoToBegin();
    it.GoToBegin();
    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        auto index = it.GetIndex();
        for (int k = 0; k < 3; ++k)
        {
            ASSERT_NEAR(value[k], index[0] >= 10 && index[0] < 15 && index[1] >= 10 && index[1] < 15 ? 1 : 0, 0.0001);
        }
        ++it;
    }
}
