/**
 * \file median.h
 */

#pragma once

#include <vector>

namespace astro
{
namespace filters
{
namespace stackers
{
template<typename Type>
class Median
{
public:
    Type operator()(std::vector<Type>& values) const
    {
        std::partial_sort(values.begin(), values.begin() + (values.size() + 1) / 2, values.end());
        return values[(values.size() + 1) / 2 - 1];
        
    }
};
} // namespace stackers
} // namespace filters
} // namespace astro
