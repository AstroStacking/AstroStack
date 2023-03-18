#pragma once

#include <Algos/ImageStats/traits.h>
#include <Algos/config.h>
#include <IO/io.h>

#include <vector>

namespace astro
{
namespace image_statistics
{
/// Computes the mean of a label
class Mean
{
    /// Cumulative values
    std::vector<double> m_cumulative;
    // Number of accumulated values
    std::vector<double> m_nb;

public:
    ASTRO_ALGORITHMS_EXPORT Mean(size_t entries);

    ASTRO_ALGORITHMS_EXPORT static constexpr size_t getNbStats() { return 1; }

    ASTRO_ALGORITHMS_EXPORT void compute();
    ASTRO_ALGORITHMS_EXPORT void process(double value, int32_t index, ScalarIntegerImageType::IndexType position);
    ASTRO_ALGORITHMS_EXPORT double getData(size_t index, size_t stat)
    {
        return m_cumulative[index * getNbStats() + stat];
    }
};
} // namespace image_statistics
} // namespace astro
