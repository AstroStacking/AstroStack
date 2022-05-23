#include "doubleprogressbar.h"
#include "ui_doubleprogressbar.h"

DoubleProgressBar::DoubleProgressBar(QWidget* parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::DoubleProgressBar>())
{
    ui->setupUi(this);
}

DoubleProgressBar::~DoubleProgressBar() = default;
