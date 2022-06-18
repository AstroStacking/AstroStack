#include "io.h"

namespace astro
{
AstroImage::AstroImage() = default;
;

AstroImage::AstroImage(ImageTypePtr img /*, Exiv2::ExifData data*/)
    : m_img(std::move(img))
//, m_data(std::move(data))
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

} // namespace astro
