#include "minmax_position.h"

namespace astro
{
namespace image_statistics
{
MinMaxPosition::MinMaxPosition(size_t entries)
    : m_position(entries, {std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(),
                           std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest()})
{
}

void MinMaxPosition::process(double value, int32_t index, ScalarIntegerImageType::IndexType position)
{
    m_position[index][0] = std::min(m_position[index][0], static_cast<double>(position[0]));
    m_position[index][1] = std::max(m_position[index][1], static_cast<double>(position[0]));
    m_position[index][2] = std::min(m_position[index][2], static_cast<double>(position[1]));
    m_position[index][3] = std::max(m_position[index][3], static_cast<double>(position[1]));
}
} // namespace image_statistics
} // namespace astro
