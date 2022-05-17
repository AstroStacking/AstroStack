#include "itkInput.h"

#include <itkImageFileReader.h>

namespace astro
{
using ReaderType = itk::ImageFileReader<ImageType>;

ITKInputPlugin::~ITKInputPlugin() = default;

QStringList ITKInputPlugin::filters() const
{
    return {"jpg", "png", "tif"};
}

ImageTypePtr ITKInputPlugin::open(QString filename, QWidget* parent) const
{
    auto reader = ReaderType::New();
    
    reader->SetFileName(filename.toStdString());
    reader->Update();
    return reader->GetOutput();
}

}
