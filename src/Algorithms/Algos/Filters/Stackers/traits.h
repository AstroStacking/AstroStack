#pragma once

#include <IO/io.h>

#include <itkFixedArray.h>

#include <cstddef>

namespace astro
{
namespace filters
{
namespace stackers
{
template<typename T>
struct Traits
{
};

template<>
struct Traits<float>
{
    static bool isnan(float x) { return std::isnan(x); }
};

template<typename T, size_t Size>
struct Traits<itk::FixedArray<T, Size>>
{
    static bool isnan(itk::FixedArray<T, Size> x)
    {
        for (T el : x)
        {
            if (Traits<T>::isnan(x))
            {
                return true;
            }
        }
        return false;
    }
};
} // namespace stackers
} // namespace filters
} // namespace astro
