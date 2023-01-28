#include "size.h"

namespace astro
{
namespace image_statistics
{
Size::Size(size_t entries)
    : m_cumulative(entries)
{
}

void Size::process(double value, int32_t index, ScalarIntegerImageType::IndexType position)
{
    m_cumulative[index] += value;
}
} // namespace image_statistics
} // namespace astro
