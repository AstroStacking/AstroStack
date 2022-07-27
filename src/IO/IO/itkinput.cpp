#include "itkinput.h"

#include <QtCore/QFileInfo>
#include <QtWidgets/QMessageBox>

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
    return {"jpg", "png", "tif", "tiff", "JPG", "PNG", "TIF", "TIFF"};
}

ImageTypePtr ITKInputPlugin::open(QString filename, QWidget* parent) const
try
{
    QFileInfo info(filename);
    QString extension = info.completeSuffix();

    auto reader = ReaderType::New();

    reader->SetFileName(filename.toStdString());
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
catch (...)
{
    QMessageBox msgBox;
    msgBox.setText(tr("Could not load image ") + filename + ".");
    msgBox.exec();
    return ImageTypePtr();
}
} // namespace astro
