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
using UnderlyingPixelType = float;
using UnderlyingScalarPixelType = uint32_t;
constexpr unsigned int Dimension = 2;
using PixelType = itk::RGBPixel<UnderlyingPixelType>;
constexpr unsigned int PixelDimension = PixelType::Length;

using ImageType = itk::Image<PixelType, Dimension>;
using ScalarImageType = itk::Image<UnderlyingPixelType, Dimension>;
using ScalarIntegerImageType = itk::Image<UnderlyingScalarPixelType, Dimension>;
using ImageTypePtr = itk::SmartPointer<ImageType>;
using ScalarImageTypePtr = itk::SmartPointer<ScalarImageType>;
using ScalarIntegerImageTypePtr = itk::SmartPointer<ScalarIntegerImageType>;

class RedChannelPixelAccessor
{
public:
    using InternalType = PixelType;
    using ExternalType = UnderlyingPixelType;

    static ExternalType Get(const InternalType& input) { return static_cast<ExternalType>(input.GetRed()); }
};

class GreenChannelPixelAccessor
{
public:
    using InternalType = PixelType;
    using ExternalType = UnderlyingPixelType;

    static ExternalType Get(const InternalType& input) { return static_cast<ExternalType>(input.GetGreen()); }
};

class BlueChannelPixelAccessor
{
public:
    using InternalType = PixelType;
    using ExternalType = UnderlyingPixelType;

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

ASTRO_IO_EXPORT AstroImage enrichImage(const std::string& filename, ImageTypePtr img);
ASTRO_IO_EXPORT void saveEnrichedImage(const std::string& filename, const AstroImage& img);
} // namespace astro
