/**
 * \file max.h
 */

#pragma once

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
    Type operator()(const std::vector<Type>& values) const { return *std::max_element(values.begin(), values.end()); }
};
} // namespace stackers
} // namespace filters
} // namespace astro
