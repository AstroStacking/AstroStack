#pragma once
#include <itkImage.h>
#include <itkVectorImage.h>

namespace astro
{
using PixelType = float;
constexpr unsigned int Dimension = 2;
using ImageType = itk::VectorImage<PixelType, Dimension>;
using ScalarImageType = itk::Image<PixelType, Dimension>;
using ImageTypePtr = itk::SmartPointer<ImageType>;
using ScalarImageTypePtr = itk::SmartPointer<ScalarImageType>;
} // namespace astro
