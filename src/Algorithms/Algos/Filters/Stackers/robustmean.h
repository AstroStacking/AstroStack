/**
 * \file robustmean.h
 */

#pragma once

#include <Algos/Filters/Stackers/traits.h>

#include <iostream>
#include <numeric>
#include <vector>

namespace astro
{
namespace filters
{
namespace stackers
{
template<typename Type>
class RobustMean
{
    double ratio;

public:
    RobustMean(double ratio)
        : ratio(ratio)
    {
    }

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

        size_t pivot = static_cast<size_t>(std::floor(nanvalues.size() * ratio));
        std::partial_sort(nanvalues.begin(), nanvalues.begin() + pivot, nanvalues.end());
        std::partial_sort(nanvalues.rbegin(), nanvalues.rbegin() + pivot, nanvalues.rend() - pivot,
                          std::greater<Type>());
        return std::accumulate(nanvalues.begin() + pivot, nanvalues.end() - pivot, 0.) / (nanvalues.size() - 2 * pivot);
    }
};
} // namespace stackers
} // namespace filters
} // namespace astro
