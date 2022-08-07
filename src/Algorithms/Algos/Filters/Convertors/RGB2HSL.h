/**
 * \file RGB2HSL.h
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

template<typename TInput, typename TOutput>
class RGB2HSLFunctor
{
public:
    RGB2HSLFunctor() = default;
    ~RGB2HSLFunctor() = default;
    bool operator==(const RGB2HSLFunctor&) const { return true; }
    ITK_UNEQUAL_OPERATOR_MEMBER_FUNCTION(RGB2HSLFunctor);

    inline TOutput operator()(const TInput& A) const { return static_cast<TOutput>(RGB2HSLFunctor(A)); }
};

template<typename TInput, typename TOutput>
class RGB2LFunctor
{
public:
    RGB2LFunctor() = default;
    ~RGB2LFunctor() = default;
    bool operator==(const RGB2LFunctor&) const { return true; }
    ITK_UNEQUAL_OPERATOR_MEMBER_FUNCTION(RGB2LFunctor);

    inline TOutput operator()(const TInput& A) const { return static_cast<TOutput>(RGB2LFunctor(A)); }
};

} // namespace convertors
} // namespace filters
} // namespace astro
