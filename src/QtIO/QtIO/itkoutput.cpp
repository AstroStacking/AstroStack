#include "itkoutput.h"

#include <IO/itkoutput.h>

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
    return {"jpg", "png", "tif", "tiff"};
}

void ITKOutputPlugin::save(ImageTypePtr img, QString filename, QWidget* parent) const
try
{
    io::save(img, filename.toStdString());
}
catch (...)
{
    QMessageBox msgBox;
    msgBox.setText(tr("Could not save image ") + filename + ".");
    msgBox.exec();
}
} // namespace astro
