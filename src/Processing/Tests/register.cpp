#include <IO/itkinput.h>
#include <Processing/register.h>

#include <gtest/gtest.h>

#include <itkImageRegionIterator.h>

TEST(Processing, RegisterGraph)
{
    auto fix = astro::io::open("square1.png");
    auto moving = astro::io::open("square2.png");

    constexpr double limit = 0.1;

    auto result = astro::processing::registerImages(
            fix, moving, {{0, 0}, {0, 10}, {0, 20}, {10, 0}, {10, 10}, {10, 20}, {20, 0}, {20, 10}, {20, 20}},
            {{5, 10}, {5, 20}, {5, 30}, {15, 10}, {15, 20}, {15, 30}, {25, 10}, {25, 20}, {25, 30}}, 0);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(result, result->GetRequestedRegion());
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

TEST(Processing, RegisterDirect)
{
    auto fix = astro::io::open("square1.png");
    auto moving = astro::io::open("square2.png");

    auto result = astro::processing::registerImages(fix, moving, 0);

    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(result, result->GetRequestedRegion());
    it.GoToBegin();
    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        auto index = it.GetIndex();
        std::cout << index << ": " << value << std::endl;
        ++it;
    }
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
