#include "grey.h"

#include <IO/hdf5.h>
#include <IO/traits.h>

namespace astro
{
namespace processing
{
ImageTypePtr grey(const H5::DataSet& input, size_t index, H5::Group& output, const std::string& dataset)
{
    ImageTypePtr img = hdf5::extractFrom(input, index);

    // greying

    hdf5::writeTo(img, output, dataset);

    return img;
}
} // namespace processing
} // namespace astro
