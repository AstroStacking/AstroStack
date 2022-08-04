/**
 * \file max.h
 */

#pragma once

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

    Type operator()(std::vector<Type>& values) const { return values[0]; }
};
} // namespace stackers
} // namespace filters
} // namespace astro
