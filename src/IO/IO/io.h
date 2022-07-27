#pragma once

#include <config.h>

#include <IO/config.h>

#include <itkImage.h>
#include <itkRGBPixel.h>

#if ASTRO_HAVE_EXIV2
#include <exiv2/exiv2.hpp>
#endif

namespace astro
{
using PixelType = float;
constexpr unsigned int Dimension = 2;
constexpr unsigned int PixelDimension = 3;
using InternalImageType = itk::RGBPixel<PixelType>;
using ImageType = itk::Image<itk::RGBPixel<PixelType>, Dimension>;
using ScalarImageType = itk::Image<PixelType, Dimension>;
using ImageTypePtr = itk::SmartPointer<ImageType>;
using ScalarImageTypePtr = itk::SmartPointer<ScalarImageType>;

class RedChannelPixelAccessor
{
public:
    using InternalType = InternalImageType;
    using ExternalType = PixelType;

    static ExternalType Get(const InternalType& input) { return static_cast<ExternalType>(input.GetRed()); }
};

class GreenChannelPixelAccessor
{
public:
    using InternalType = InternalImageType;
    using ExternalType = PixelType;

    static ExternalType Get(const InternalType& input) { return static_cast<ExternalType>(input.GetGreen()); }
};

class BlueChannelPixelAccessor
{
public:
    using InternalType = InternalImageType;
    using ExternalType = PixelType;

    static ExternalType Get(const InternalType& input) { return static_cast<ExternalType>(input.GetBlue()); }
};

class ASTRO_IO_EXPORT AstroImage
{
    ImageTypePtr m_img;
#if ASTRO_HAVE_EXIV2
    Exiv2::ExifData m_data;
#endif
public:
    AstroImage();
    explicit AstroImage(ImageTypePtr img
#if ASTRO_HAVE_EXIV2
                        ,
                        Exiv2::ExifData data
#endif
    );
    bool isValid() const;
    void setImg(ImageTypePtr img);
    const ImageTypePtr& getImg() const;
#if ASTRO_HAVE_EXIV2
    const Exiv2::ExifData& getExif() const;
#endif
};
} // namespace astro
