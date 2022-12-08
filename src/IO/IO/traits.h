#pragma once

#include <IO/io.h>

namespace astro
{
template<typename ImageType>
struct Traits
{
};

template<>
struct Traits<ImageType>
{
    static constexpr size_t NbDimensions = 3;
    static constexpr size_t LastDim = PixelDimension;
    using PixelType = astro::PixelType;
};

template<>
struct Traits<ScalarImageType>
{
    static constexpr size_t NbDimensions = 2;
    using PixelType = UnderlyingPixelType;
};

} // namespace astro
