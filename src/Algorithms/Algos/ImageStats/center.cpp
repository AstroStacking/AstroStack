#include "center.h"

namespace astro
{
namespace image_statistics
{
Center::Center(size_t entries)
    : m_cumulative(entries)
    , m_nb(entries)
{
}

void Center::compute()
{
    for (size_t i = 0; i < m_cumulative.size(); ++i)
    {
        m_cumulative[i][0] /= m_nb[i];
        m_cumulative[i][1] /= m_nb[i];
    }
}

void Center::process(double value, int32_t index, ScalarIntegerImageType::IndexType position)
{
    m_nb[index] += value;
    m_cumulative[index][0] += value * position[0];
    m_cumulative[index][1] += value * position[1];
}
} // namespace image_statistics
} // namespace astro
