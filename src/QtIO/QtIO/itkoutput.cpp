#include "itkoutput.h"

#include <IO/itkoutput.h>

#include <QtCore/QFileInfo>
#include <QtWidgets/QMessageBox>

#include <itkCastImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkMultiplyImageFilter.h>

namespace astro
{
ITKOutputPlugin::~ITKOutputPlugin() = default;

QStringList ITKOutputPlugin::filters() const
{
    return {"jpg", "png", "tif", "tiff"};
}

void ITKOutputPlugin::save(ImageTypePtr img, QString filename, QWidget* parent) const
try
{
    io::save<uint16_t>(img, filename.toStdString());
}
catch (...)
{
    try
    {
        io::save<uint8_t>(img, filename.toStdString());
    }
    catch (...)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Could not save image ") + filename + ".");
        msgBox.exec();
    }
    QMessageBox msgBox;
    msgBox.setText(tr("Could not save image ") + filename + ".");
    msgBox.exec();
}
} // namespace astro
