#include "io.h"

namespace astro
{
AstroImage::AstroImage() = default;
;

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
} // namespace astro
