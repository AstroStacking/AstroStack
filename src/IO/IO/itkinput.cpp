#include "itkinput.h"

#include <QtCore/QFileInfo>
#include <QtWidgets/QMessageBox>

#include <itkImageFileReader.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkMultiplyImageFilter.h>
#include <itkVectorIndexSelectionCastImageFilter.h>

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
    return {"jpg", "png", "tif"};
}

ImageTypePtr ITKInputPlugin::open(QString filename, QWidget* parent) const
try
{
    QFileInfo info(filename);
    QString extension = info.completeSuffix();

    auto reader = ReaderType::New();

    reader->SetFileName(filename.toStdString());
    reader->Update();

    using IndexSelectionType = itk::VectorIndexSelectionCastImageFilter<ImageType, ScalarImageType>;
    auto indexSelectionFilter = IndexSelectionType::New();
    indexSelectionFilter->SetIndex(0);
    indexSelectionFilter->SetInput(reader->GetOutput());
    indexSelectionFilter->Update();

    using ImageCalculatorFilterType = itk::MinimumMaximumImageCalculator<ScalarImageType>;

    auto imageCalculatorFilter = ImageCalculatorFilterType::New();
    imageCalculatorFilter->SetImage(indexSelectionFilter->GetOutput());
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
