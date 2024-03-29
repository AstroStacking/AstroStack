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
    filter->SetConstant(static_cast<UnderlyingPixelType>(std::numeric_limits<OutputType>::max()));
    filter->Update();

    using OutputImageType = itk::Image<itk::RGBPixel<OutputType>, Dimension>;
    using CastFilterType = itk::CastImageFilter<ImageType, OutputImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(filter->GetOutput());
    castFilter->Update();

    using WriterType = itk::ImageFileWriter<OutputImageType>;
    auto writer = WriterType::New();
    writer->SetInput(castFilter->GetOutput());
    writer->SetFileName(filename);
    writer->Update();
}

template<typename OutputType>
void save(const ScalarImageTypePtr& img, const std::string& filename)
{
    using FilterType = itk::MultiplyImageFilter<ScalarImageType, ScalarImageType, ScalarImageType>;
    auto filter = FilterType::New();
    filter->SetInput(img);
    filter->SetConstant(static_cast<UnderlyingPixelType>(std::numeric_limits<OutputType>::max()));
    filter->Update();

    using OutputImageType = itk::Image<OutputType, Dimension>;
    using CastFilterType = itk::CastImageFilter<ScalarImageType, OutputImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(filter->GetOutput());
    castFilter->Update();

    using WriterType = itk::ImageFileWriter<OutputImageType>;
    auto writer = WriterType::New();
    writer->SetInput(castFilter->GetOutput());
    writer->SetFileName(filename);
    writer->Update();
}

template void ASTRO_IO_EXPORT save<uint8_t>(const ImageTypePtr& img, const std::string& filename);
template void ASTRO_IO_EXPORT save<uint16_t>(const ImageTypePtr& img, const std::string& filename);
template void ASTRO_IO_EXPORT save<uint8_t>(const ScalarImageTypePtr& img, const std::string& filename);
template void ASTRO_IO_EXPORT save<uint16_t>(const ScalarImageTypePtr& img, const std::string& filename);
} // namespace io
} // namespace astro
