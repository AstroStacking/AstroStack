#include "traits.h"

namespace astro
{
const H5::PredType& DataTraits<double>::HDF5Type = H5::PredType::NATIVE_DOUBLE;
const H5::PredType& DataTraits<float>::HDF5Type = H5::PredType::NATIVE_FLOAT;
const H5::PredType& DataTraits<uint64_t>::HDF5Type = H5::PredType::NATIVE_UINT64;
} // namespace astro
