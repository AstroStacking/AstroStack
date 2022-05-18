#include "itkInput.h"

#include <itkImageFileReader.h>

namespace itk
{
void JPEGImageIOFactoryRegister__Private();
void PNGImageIOFactoryRegister__Private();
void TIFFImageIOFactoryRegister__Private();
}

namespace astro
{
using ReaderType = itk::ImageFileReader<ImageType>;

ITKInputPlugin::ITKInputPlugin()
{
    itk::JPEGImageIOFactoryRegister__Private();
    itk::PNGImageIOFactoryRegister__Private();
    itk::TIFFImageIOFactoryRegister__Private();
}

ITKInputPlugin::~ITKInputPlugin() = default;

QStringList ITKInputPlugin::filters() const
{
    return {"jpg", "png", "tif"};
}

ImageTypePtr ITKInputPlugin::open(QString filename, QWidget* parent) const
try {
    auto reader = ReaderType::New();
    
    reader->SetFileName(filename.toStdString());
    reader->Update();
    return reader->GetOutput();
}
catch(...)
{
    return ImageTypePtr();
}
}
