/**
 * \file robustmean.h
 */

#pragma once

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
    RobustMean(double ratio):
    ratio(ratio)
    {}
    
    Type operator()(std::vector<Type>& values) const
    {
        size_t pivot = static_cast<size_t>(std::floor(values.size() * ratio));
        std::partial_sort(values.begin(), values.begin() + pivot, values.end());
        std::partial_sort(values.rbegin(), values.rbegin() + + pivot, values.rend() - pivot, std::greater<Type>());
        std::cout << std::endl;
        return std::accumulate(values.begin() + pivot, values.end() - pivot, 0.) / (values.size() - 2 * pivot);
    }
};
} // namespace stackers
} // namespace filters
} // namespace astro
