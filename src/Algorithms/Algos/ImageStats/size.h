#pragma once

#include <Algos/ImageStats/traits.h>
#include <IO/io.h>

#include <vector>

namespace astro
{
namespace image_statistics
{
/// Computes the size of a label
class Size
{
    /// Cumulative values
    std::vector<double> m_cumulative;

public:
    Size(size_t entries);

    static constexpr size_t getNbStats() { return 1; }

    void compute() {}
    void process(double value, int32_t index, ScalarIntegerImageType::IndexType position);
    double getData(size_t index, size_t stat) { return m_cumulative[index * getNbStats() + stat]; }
};
} // namespace image_statistics
} // namespace astro
