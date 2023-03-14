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
class Max
{
public:
    Type operator()(const std::vector<Type>& values) const
    {
        return *std::max_element(values.begin(), values.end(),
                                 [](Type x, Type other)
                                 {
                                     if (Traits<Type>::isnan(x))
                                     {
                                         return true;
                                     }
                                     if (Traits<Type>::isnan(other))
                                     {
                                         return false;
                                     }
                                     return x < other;
                                 });
    }
};
} // namespace stackers
} // namespace filters
} // namespace astro
