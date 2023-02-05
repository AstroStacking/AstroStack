#pragma once

#include <IO/io.h>
#include <Processing/config.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ImageTypePtr registerImages(const ImageTypePtr& fix, const ImageTypePtr& moving,
                                                    const std::vector<std::pair<double, double>> fixStars,
                                                    const std::vector<std::pair<double, double>> movingStars,
                                                    double defaultValue = std::numeric_limits<double>::quiet_NaN());
ASTRO_PROCESSING_EXPORT ImageTypePtr registerImages(const ImageTypePtr& fix, const ImageTypePtr& target,
                                                    double defaultValue = std::numeric_limits<double>::quiet_NaN());
ASTRO_PROCESSING_EXPORT ImageTypePtr
registerImagesHighQuality(const ImageTypePtr& fix, const ImageTypePtr& moving,
                          double defaultValue = std::numeric_limits<double>::quiet_NaN());
} // namespace processing
} // namespace astro
