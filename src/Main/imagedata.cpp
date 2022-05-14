#include "imagedata.h"
#include "ui_imagedata.h"

#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QModelIndex>

#include <iostream>

ImageData::ImageData(QWidget* parent)
    : QFrame(parent), m_ui(std::make_unique<Ui::ImageData>())
{
    m_ui->setupUi(this);
    m_scene = new QGraphicsScene;
    m_ui->graphicsView->setScene(m_scene);
}

ImageData::~ImageData() = default;

void ImageData::doubleClicked(const QModelIndex& index)
{
    QModelIndex current = index;
    QString path(current.data().toString());
    while (current.parent() != QModelIndex())
    {
        current = current.parent();
        path = current.data().toString() + "/" + path;
    }
    std::cout <<"test" << path.toStdString()<<std::endl;
    QImage image(path);
    m_scene->addItem(
        new QGraphicsPixmapItem(QPixmap::fromImage(image)));
}
