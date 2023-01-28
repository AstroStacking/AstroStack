#pragma once

#include <Algos/ImageStats/traits.h>
#include <IO/io.h>

#include <vector>

namespace astro
{
namespace image_statistics
{
/**
 * Computes statistics over a labeled image
 */
template<class... Stats>
class ImageStatistics : public Stats...
{
    std::vector<double> m_data;
    const size_t m_entries;

public:
    ImageStatistics(size_t entries)
        : Stats(entries)...
        , m_data(entries * Traits<Stats...>::getNbStats())
        , m_entries(entries)
    {
    }

    void process(double value, int32_t index, ScalarIntegerImageType::IndexType position)
    {
        if (index != 0)
        {
            Traits<Stats...>::process(*this, value, index - 1, position);
        }
    }

    void compute() { Traits<Stats...>::compute(*this, m_entries); }

    size_t getSize() const { return m_entries; }
    const std::vector<double>& getData() const { return m_data; }
    void setData(double value, size_t index) { m_data[index] = value; }

    //double get
};
} // namespace image_statistics
} // namespace astro
