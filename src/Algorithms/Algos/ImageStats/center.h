#pragma once

#include <Algos/ImageStats/traits.h>
#include <Algos/config.h>
#include <IO/io.h>

#include <array>
#include <vector>

namespace astro
{
namespace image_statistics
{
/// Computes the center of a label
class Center
{
    /// Cumulative values
    std::vector<std::array<double, 2>> m_cumulative;
    // Number of accumulated values
    std::vector<double> m_nb;

public:
    ASTRO_ALGORITHMS_EXPORT Center(size_t entries);

    ASTRO_ALGORITHMS_EXPORT static constexpr size_t getNbStats() { return 2; }

    ASTRO_ALGORITHMS_EXPORT void compute();
    ASTRO_ALGORITHMS_EXPORT void process(double value, int32_t index, ScalarIntegerImageType::IndexType position);
    ASTRO_ALGORITHMS_EXPORT double getData(size_t index, size_t stat) { return m_cumulative[index][stat]; }
};
} // namespace image_statistics
} // namespace astro
