#include "itkoutput.h"

#include <itkCastImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkMultiplyImageFilter.h>

namespace itk
{
void JPEGImageIOFactoryRegister__Private();
void PNGImageIOFactoryRegister__Private();
void TIFFImageIOFactoryRegister__Private();
} // namespace itk

namespace astro
{
namespace io
{
template<typename OutputType>
void save(const ImageTypePtr& img, const std::string& filename)
{
    using FilterType = itk::MultiplyImageFilter<ImageType, ScalarImageType, ImageType>;
    auto filter = FilterType::New();
    filter->SetInput(img);
    filter->SetConstant(std::numeric_limits<OutputType>::max());
    filter->Update();

    using OutputImageType = itk::Image<itk::RGBPixel<uint8_t>, Dimension>;
    using CastFilterType = itk::CastImageFilter<ImageType, OutputImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(filter->GetOutput());

    using WriterType = itk::ImageFileWriter<OutputImageType>;
    auto writer = WriterType::New();
    writer->SetInput(castFilter->GetOutput());
    writer->SetFileName(filename);
    writer->Update();
}
template void save<uint8_t>(const ImageTypePtr& img, const std::string& filename);
template void save<uint16_t>(const ImageTypePtr& img, const std::string& filename);

} // namespace io
} // namespace astro
