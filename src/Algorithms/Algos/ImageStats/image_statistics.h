#pragma once

#include <Algos/ImageStats/traits.h>
#include <IO/io.h>

#include <vector>

namespace astro
{
namespace image_statistics
{
template<class... Stats>
class ImageStatistics : public Stats...
{
    std::vector<double> m_data;

public:
    ImageStatistics(size_t entries)
        : Stats(entries)...
        , m_data(entries * Traits<Stats...>::getNbStats())
    {
    }

    void process(double value, int32_t index, ScalarIntegerImageType::IndexType position)
    {
        if (index != 0)
        {
            Traits<Stats...>::process(*this, value, index - 1, position);
        }
    }

    void compute()
    {
        Traits<Stats...>::compute(*this);
    }

    //double get
};
} // namespace image_statistics
} // namespace astro
