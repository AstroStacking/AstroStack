#pragma once

#include <QWidget>

namespace Ui
{
class Explorer;
}

class Explorer : public QWidget
{
    Q_OBJECT

  public:
    explicit Explorer(QWidget* parent = nullptr);
    ~Explorer();

  private:
    std::unique_ptr<Ui::Explorer> ui;
};
