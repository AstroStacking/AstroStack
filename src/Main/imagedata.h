#pragma once
#include <QFrame>

namespace Ui {
class ImageData;
}

class ImageData : public QFrame
{
    Q_OBJECT

public:
    explicit ImageData(QWidget *parent = nullptr);
    ~ImageData();

private:
    std::unique_ptr<Ui::ImageData> ui;
};

