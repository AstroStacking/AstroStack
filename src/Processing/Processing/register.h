#pragma once

#include <IO/io.h>
#include <Processing/config.h>

namespace astro
{
namespace processing
{
ASTRO_PROCESSING_EXPORT ImageTypePtr registerImages(const ImageTypePtr& ref, const ImageTypePtr& target,
                                                    const std::vector<std::pair<double, double>> refStars,
                                                    const std::vector<std::pair<double, double>> targetStars,
                                                    double defaultValue = std::numeric_limits<double>::quiet_NaN());
ASTRO_PROCESSING_EXPORT ImageTypePtr registerImagesHighQuality(
        const ImageTypePtr& ref, const ImageTypePtr& target, const std::vector<std::pair<double, double>> refStars,
        const std::vector<std::pair<double, double>> targetStars,
        double defaultValue = std::numeric_limits<double>::quiet_NaN());
} // namespace processing
} // namespace astro
