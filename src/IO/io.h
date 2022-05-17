#pragma once
#include "itkImage.h"

namespace astro
{
using PixelType = float;
constexpr unsigned int Dimension = 2;
using ImageType = itk::Image<PixelType, Dimension>;
using ImageTypePtr = itk::SmartPointer<ImageType>;
}
