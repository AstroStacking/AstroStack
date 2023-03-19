/**
 * \file max.h
 */

#pragma once

#include <Algos/Filters/Stackers/traits.h>

#include <numeric>
#include <vector>

namespace astro
{
namespace filters
{
namespace stackers
{
template<typename Type>
class Count
{
public:
    Type operator()(const std::vector<Type>& values) const
    {
        return static_cast<Type>(std::accumulate(values.begin(), values.end(), 0U,
                                                 [](size_t value, Type other)
                                                 { return value + !Traits<Type>::isnan(other); })) /
               values.size();
    }
};
} // namespace stackers
} // namespace filters
} // namespace astro
