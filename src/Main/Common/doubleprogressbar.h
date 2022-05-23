#pragma once

#include <QDialog>

namespace Ui
{
class DoubleProgressBar;
}

class DoubleProgressBar : public QDialog
{
    Q_OBJECT

public:
    explicit DoubleProgressBar(QWidget* parent = nullptr);
    ~DoubleProgressBar() override;

private:
    std::unique_ptr<Ui::DoubleProgressBar> ui;
};
