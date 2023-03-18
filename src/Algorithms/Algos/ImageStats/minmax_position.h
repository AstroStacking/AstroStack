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
/// Computes the min/max pixels of the label
class MinMaxPosition
{
    /// (minX, maxX, minY, maxY)
    std::vector<std::array<double, 4>> m_position;

public:
    ASTRO_ALGORITHMS_EXPORT MinMaxPosition(size_t entries);

    ASTRO_ALGORITHMS_EXPORT static constexpr size_t getNbStats() { return 4; }

    ASTRO_ALGORITHMS_EXPORT void compute() {}
    ASTRO_ALGORITHMS_EXPORT void process(double value, int32_t index, ScalarIntegerImageType::IndexType position);
    ASTRO_ALGORITHMS_EXPORT double getData(size_t index, size_t stat) { return m_position[index][stat]; }
};
} // namespace image_statistics
} // namespace astro
