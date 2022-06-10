#include "itkoutput.h"

#include <QtCore/QFileInfo>
#include <QtWidgets/QMessageBox>

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
ITKOutputPlugin::ITKOutputPlugin()
{
    itk::JPEGImageIOFactoryRegister__Private();
    itk::PNGImageIOFactoryRegister__Private();
    itk::TIFFImageIOFactoryRegister__Private();
}

ITKOutputPlugin::~ITKOutputPlugin() = default;

QStringList ITKOutputPlugin::filters() const
{
    return {"jpg", "png", "tif"};
}

void ITKOutputPlugin::save(ImageTypePtr img, QString filename, QWidget* parent) const
try
{
    using FilterType = itk::MultiplyImageFilter<ImageType, ScalarImageType, ImageType>;
    auto filter = FilterType::New();
    filter->SetInput(img);
    filter->SetConstant(255.);
    filter->Update();

    using OutputImageType = itk::VectorImage<uint8_t, Dimension>;
    using CastFilterType = itk::CastImageFilter<ImageType, OutputImageType>;
    auto castFilter = CastFilterType::New();
    castFilter->SetInput(filter->GetOutput());

    using WriterType = itk::ImageFileWriter<OutputImageType>;
    auto writer = WriterType::New();
    writer->SetInput(castFilter->GetOutput());
    writer->SetFileName(filename.toStdString());
    writer->Update();
}
catch (...)
{
    QMessageBox msgBox;
    msgBox.setText(tr("Could not save image ") + filename + ".");
    msgBox.exec();
}
} // namespace astro
