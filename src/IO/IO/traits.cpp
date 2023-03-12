#include "traits.h"

namespace astro
{
const H5::PredType& DataTraits<double>::HDF5Type = H5::PredType::NATIVE_DOUBLE;
const H5::PredType& DataTraits<unsigned long>::HDF5Type = H5::PredType::NATIVE_ULONG;
} // namespace astro
