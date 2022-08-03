/**
 * \file HSL2RGB.h
 */

#pragma once

#include <IO/io.h>

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
} // namespace convertors
} // namespace filters
} // namespace astro
