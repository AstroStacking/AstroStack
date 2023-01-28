#include "mean.h"

namespace astro
{
namespace image_statistics
{
Mean::Mean(size_t entries)
    : m_cumulative(entries)
    , m_nb(entries)
{
}

void Mean::compute()
{
    for (size_t i = 0; i < m_cumulative.size(); ++i)
    {
        m_cumulative[i] /= m_nb[i];
    }
}

void Mean::process(double value, int32_t index, ScalarIntegerImageType::IndexType position)
{
    ++m_nb[index];
    m_cumulative[index] += value;
}
} // namespace image_statistics
} // namespace astro
