#pragma once

#include <Algos/ImageStats/traits.h>
#include <IO/io.h>

#include <vector>

namespace astro
{
namespace image_statistics
{
/// Computes the mean of an image
class Mean
{
    ///
    std::vector<double> m_cumulative;
    std::vector<double> m_nb;

public:
    Mean(size_t entries);


    static constexpr size_t getNbStats() { return 1; }

    void compute();
    void process(double value, int32_t index, ScalarIntegerImageType::IndexType position);
};
} // namespace image_statistics
} // namespace astro
