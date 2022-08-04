/**
 * \file max.h
 */

#pragma once

#include <IO/io.h>

#include <itkRGBPixel.h>

namespace astro
{
namespace filters
{
namespace stackers
{
template<typename TInput, typename TOutput = TInput1>
class Max
{
public:
    Max() = default;
    ~Max() = default;
    bool operator==(const Max&) const { return true; }

    ITK_UNEQUAL_OPERATOR_MEMBER_FUNCTION(Max);

    TOutput operator()(std::vector<itk::RGBPixel<Type>>& values) const
    {
        return static_cast<TOutput>(values[0]);
    }
};
} // namespace stackers
} // namespace filters
} // namespace astro
