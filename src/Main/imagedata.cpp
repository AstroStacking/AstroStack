#include "imagedata.h"
#include "ui_imagedata.h"

ImageData::ImageData(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ImageData)
{
    ui->setupUi(this);
}

ImageData::~ImageData() = default;
