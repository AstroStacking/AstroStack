#include "itkinput.h"

#include <IO/itkinput.h>

#include <QtCore/QFileInfo>
#include <QtWidgets/QMessageBox>

namespace itk
{
void JPEGImageIOFactoryRegister__Private();
void PNGImageIOFactoryRegister__Private();
void TIFFImageIOFactoryRegister__Private();
} // namespace itk

namespace astro
{
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
    return io::open(filename.toStdString());
}
catch (...)
{
    QMessageBox msgBox;
    msgBox.setText(tr("Could not load image ") + filename + ".");
    msgBox.exec();
    return ImageTypePtr();
}
} // namespace astro
