#include "itkinput.h"

#include <itkImageAdaptor.h>
#include <itkImageFileReader.h>
#include <itkMinimumMaximumImageCalculator.h>
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
ImageTypePtr open(const std::string& filename)
{
    using ReaderType = itk::ImageFileReader<ImageType>;
    auto reader = ReaderType::New();

    reader->SetFileName(filename);
    reader->Update();

    using IndexSelectionType = itk::ImageAdaptor<ImageType, RedChannelPixelAccessor>;
    auto indexSelectionFilter = IndexSelectionType::New();
    indexSelectionFilter->SetImage(reader->GetOutput());
    indexSelectionFilter->Update();

    using ImageCalculatorFilterType = itk::MinimumMaximumImageCalculator<IndexSelectionType>;

    auto imageCalculatorFilter = ImageCalculatorFilterType::New();
    imageCalculatorFilter->SetImage(indexSelectionFilter);
    imageCalculatorFilter->ComputeMaximum();
    using FilterType = itk::MultiplyImageFilter<ImageType, ScalarImageType, ImageType>;
    auto filter = FilterType::New();
    filter->SetInput(reader->GetOutput());
    filter->SetConstant(imageCalculatorFilter->GetMaximum() > 255 ? 1 / 65535. : 1 / 255.);
    filter->Update();
    return filter->GetOutput();
}

bool registerITKPlugins()
{
    itk::JPEGImageIOFactoryRegister__Private();
    itk::PNGImageIOFactoryRegister__Private();
    itk::TIFFImageIOFactoryRegister__Private();
    return true;
}

bool registeredPlugins{registerITKPlugins()};

} // namespace io
} // namespace astro
