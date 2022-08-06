#include "io.h"

namespace astro
{
AstroImage::AstroImage() = default;


AstroImage::AstroImage(ImageTypePtr img
#if ASTRO_HAVE_EXIV2
                       ,
                       Exiv2::ExifData data
#endif
                       )
    : m_img(std::move(img))
#if ASTRO_HAVE_EXIV2
    , m_data(std::move(data))
#endif
{
}

bool AstroImage::isValid() const
{
    return m_img != nullptr;
}

void AstroImage::setImg(ImageTypePtr img)
{
    m_img = std::move(img);
}

const ImageTypePtr& AstroImage::getImg() const
{
    return m_img;
}

#if ASTRO_HAVE_EXIV2
const Exiv2::ExifData& AstroImage::getExif() const
{
    return m_data;
}
#endif

AstroImage enrichImage(const std::string& filename, ImageTypePtr img)
{
#if ASTRO_HAVE_EXIV2
    auto image = Exiv2::ImageFactory::open(filename);
    assert(image.get() != 0);
    image->readMetadata();
#endif
    return AstroImage(std::move(img)
#if ASTRO_HAVE_EXIV2
                              ,
                      image->exifData()
#endif
    );
}

void saveEnrichedImage(const std::string& filename, const AstroImage& img)
{
#if ASTRO_HAVE_EXIV2
    auto image = Exiv2::ImageFactory::open(filename);
    assert(image.get() != 0);
    image->setExifData(img.getExif());
    image->writeMetadata();
#endif
}
} // namespace astro
