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
    static constexpr bool ConstantLastDim = true;
    static constexpr bool LastDim = PixelDimension;
};

template<>
struct Traits<ScalarImageType>
{
    static constexpr size_t NbDimensions = 2;
    static constexpr bool ConstantLastDim = false;
};

} // namespace astro
