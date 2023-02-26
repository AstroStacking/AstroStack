/**
 * \file robustmean.h
 */

#pragma once

#include <Algos/config.h>

#include <itkMacro.h>
#include <itkRGBPixel.h>

namespace astro
{
namespace filters
{
namespace stackers
{
template<typename Type>
class RobustMean
{
public:
    Type operator()(std::vector<Type>& values) const { return *std::max_element(values.begin(), values.end()); }
};
} // namespace stackers
} // namespace filters
} // namespace astro
