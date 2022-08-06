#include "itkinput.h"

#include <IO/itkinput.h>

#include <QtCore/QFileInfo>
#include <QtWidgets/QMessageBox>

namespace astro
{

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
