/**
 * \file HSL2RGB.h
 */

#pragma once

#include <Algos/config.h>
#include <IO/io.h>

#include <itkMacro.h>
#include <itkRGBPixel.h>

namespace astro
{
namespace filters
{
namespace convertors
{
template<typename Type>
itk::RGBPixel<Type> HSL2RGB(itk::RGBPixel<Type> hsl)
{
    auto C = (1 - std::abs(2 * hsl[2] - 1)) * hsl[1];
    auto X = C * (1 - std::abs(std::fmod(hsl[0] / 60, 2) - 1));
    auto m = hsl[2] - C / 2;

    itk::RGBPixel<Type> rgb(m);

    if (hsl[0] < 60)
    {
        rgb[0] += C;
        rgb[1] += X;
    }
    else if (hsl[0] < 120)
    {
        rgb[0] += X;
        rgb[1] += C;
    }
    else if (hsl[0] < 180)
    {
        rgb[1] += C;
        rgb[2] += X;
    }
    else if (hsl[0] < 240)
    {
        rgb[1] += X;
        rgb[2] += C;
    }
    else if (hsl[0] < 300)
    {
        rgb[2] += C;
        rgb[0] += X;
    }
    else
    {
        rgb[2] = X;
        rgb[0] = C;
    }

    return rgb;
}

class RGBPixelAccessor
{
public:
    using InternalType = PixelType;
    using ExternalType = PixelType;

    static ExternalType Get(const InternalType& input) { return static_cast<ExternalType>(HSL2RGB(input)); }
};

template<typename TInput, typename TOutput>
class HSL2RGBFunctor
{
public:
    HSL2RGBFunctor() = default;
    ~HSL2RGBFunctor() = default;
    bool operator==(const HSL2RGBFunctor&) const { return true; }
    ITK_UNEQUAL_OPERATOR_MEMBER_FUNCTION(HSL2RGBFunctor);

    inline TOutput operator()(const TInput& A) const { return static_cast<TOutput>(HSL2RGBFunctor(A)); }
};
} // namespace convertors
} // namespace filters
} // namespace astro
