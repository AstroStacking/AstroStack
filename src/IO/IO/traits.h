#pragma once

#include <IO/io.h>

#include <H5Cpp.h>

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

template<typename T>
struct DataTraits
{
};

template<>
struct DataTraits<double>
{
    static const H5::PredType& HDF5Type;
};

template<>
struct DataTraits<uint64_t>
{
    static const H5::PredType& HDF5Type;
};

} // namespace astro
