#include "imagedata.h"
#include "ui_imagedata.h"

ImageData::ImageData(QWidget* parent)
    : QFrame(parent), m_ui(std::make_unique<Ui::ImageData>())
{
    m_ui->setupUi(this);
}

ImageData::~ImageData() = default;
