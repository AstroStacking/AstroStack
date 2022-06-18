#pragma once

#include <IO/config.h>

#include <itkImage.h>
#include <itkVectorImage.h>

//#include <exiv2/exiv2.hpp>

namespace astro
{
using PixelType = float;
constexpr unsigned int Dimension = 2;
using ImageType = itk::VectorImage<PixelType, Dimension>;
using ScalarImageType = itk::Image<PixelType, Dimension>;
using ImageTypePtr = itk::SmartPointer<ImageType>;
using ScalarImageTypePtr = itk::SmartPointer<ScalarImageType>;

class ASTRO_IO_EXPORT AstroImage
{
    ImageTypePtr m_img;
    //Exiv2::ExifData m_data;

public:
    AstroImage();
    explicit AstroImage(ImageTypePtr img /*, Exiv2::ExifData data*/);

    bool isValid() const;
    void setImg(ImageTypePtr img);
    const ImageTypePtr& getImg() const;
    //const Exiv2::ExifData& getExif() const {return m_data;}
};
} // namespace astro
