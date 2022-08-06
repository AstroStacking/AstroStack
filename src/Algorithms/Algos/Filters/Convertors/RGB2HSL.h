/**
 * \file RGB2HSL.h
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
Type RGB2L(itk::RGBPixel<Type> rgb)
{
    auto Cmax = std::max(std::max(rgb[0], rgb[1]), rgb[2]);
    auto Cmin = std::min(std::min(rgb[0], rgb[1]), rgb[2]);

    return (Cmax + Cmin) / 2;
}

template<typename Type>
itk::RGBPixel<Type> RGB2HSL(itk::RGBPixel<Type> rgb)
{
    auto Cmax = std::max(std::max(rgb[0], rgb[1]), rgb[2]);
    auto Cmin = std::min(std::min(rgb[0], rgb[1]), rgb[2]);
    auto delta = Cmax - Cmin;

    auto L = (Cmax + Cmin) / 2;
    itk::RGBPixel<Type> hsl;
    hsl[2] = L;

    if (delta == 0)
    {
        return hsl;
    }
    auto S = (L <= 0.5) ? (delta / (Cmax + Cmin)) : (delta / (2 - Cmax - Cmin));

    Type H;

    if (rgb[0] == Cmax)
    {
        H = ((rgb[1] - rgb[2]) / 6) / delta;
    }
    else if (rgb[1] == Cmax)
    {
        H = (1.0 / 3) + ((rgb[2] - rgb[0]) / 6) / delta;
    }
    else
    {
        H = (2.0 / 3) + ((rgb[0] - rgb[1]) / 6) / delta;
    }

    hsl[0] = H * 360;
    hsl[1] = S;
    return hsl;
}

class LPixelAccessor
{
public:
    using InternalType = PixelType;
    using ExternalType = UnderlyingPixelType;

    static ExternalType Get(const InternalType& input) { return static_cast<ExternalType>(RGB2L(input)); }
};

class HSLPixelAccessor
{
public:
    using InternalType = PixelType;
    using ExternalType = PixelType;

    static ExternalType Get(const InternalType& input) { return static_cast<ExternalType>(RGB2HSL(input)); }
};

} // namespace convertors
} // namespace filters
} // namespace astro
