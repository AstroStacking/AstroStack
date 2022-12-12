#include "stardetection.h"

#include <Algos/Filters/Stackers/max.h>
#include <Algos/Filters/multifunctorimagefilter.h>
#include <IO/traits.h>

namespace astro
{
namespace processing
{
void starDetection(const H5::DataSet& input, H5::DataSet& output, const std::string& dataset)
{
    H5::DataSpace inputDataspace = input.getSpace();
    int ndimsInput = inputDataspace.getSimpleExtentNdims();
    if (ndimsInput != Traits<ScalarImageType>::NbDimensions)
    {
        throw std::runtime_error("Wrong number of input dimensions");
    }
    hsize_t dimsInput[Traits<ScalarImageType>::NbDimensions];
    ndimsInput = inputDataspace.getSimpleExtentDims(dimsInput, nullptr);
}
} // namespace processing
} // namespace astro
