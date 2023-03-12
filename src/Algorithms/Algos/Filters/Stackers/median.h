/**
 * \file median.h
 */

#pragma once

#include <Algos/Filters/Stackers/traits.h>

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
    Type operator()(const std::vector<Type>& values) const
    {
        std::vector<Type> nanvalues;
        for (Type x : values)
        {
            if (Traits<Type>::isnan(x))
            {
                continue;
            }
            nanvalues.push_back(x);
        }
        std::partial_sort(nanvalues.begin(), nanvalues.begin() + (nanvalues.size() + 1) / 2, nanvalues.end());
        return nanvalues[(nanvalues.size() + 1) / 2 - 1];
    }
};
} // namespace stackers
} // namespace filters
} // namespace astro
