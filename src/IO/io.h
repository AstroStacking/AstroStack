#pragma once
#include "itkVectorImage.h"

namespace astro
{
using PixelType = float;
constexpr unsigned int Dimension = 2;
using ImageType = itk::VectorImage<PixelType, Dimension>;
using ImageTypePtr = itk::SmartPointer<ImageType>;
}
