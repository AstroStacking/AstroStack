/**
 * \file max.h
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
class Max
{
public:
    Max() = default;
    ~Max() = default;
    bool operator==(const Max&) const { return true; }
    ITK_UNEQUAL_OPERATOR_MEMBER_FUNCTION(Max);

    Type operator()(std::vector<Type>& values) const { return *std::max_element(values.begin(), values.end()); }
};
} // namespace stackers
} // namespace filters
} // namespace astro
