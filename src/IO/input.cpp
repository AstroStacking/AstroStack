#include "input.h"

#include <itkImageFileReader.h>

namespace astro
{
using ReaderType = itk::ImageFileReader<ImageType>;

InputInterface::~InputInterface() = default;

}
