#pragma once

#include <Algos/ImageStats/traits.h>

#include <vector>

namespace astro
{
namespace image_statistics
{
template<class... Stats>
class ImageStatistics
{
    std::vector<double> m_data;

public:
    ImageStatistics(size_t entries)
        : m_data(entries * Traits<Stats...>::getNbStats())
    {
    }

    //double get
};
} // namespace image_statistics
} // namespace astro
