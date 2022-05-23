#include "doubleprogressbar.h"
#include "ui_doubleprogressbar.h"

DoubleProgressBar::DoubleProgressBar(int tasks, QWidget* parent)
    : QDialog(parent)
    , m_ui(std::make_unique<Ui::DoubleProgressBar>())
{
    m_ui->setupUi(this);
    m_ui->totalTasksBar->setMaximum(tasks);
    m_ui->totalTasksLabel->setText(tr("Task ") + QString::number(m_ui->totalTasksBar->value()) + "/" +
                                   QString::number(tasks));

    connect(m_ui->cancel, &QPushButton::clicked, this, &DoubleProgressBar::cancel);
}

void DoubleProgressBar::startNewTask()
{
    m_ui->totalTasksBar->setValue(m_ui->totalTasksBar->value() + 1);
    m_ui->totalTasksLabel->setText(tr("Task ") + QString::number(m_ui->totalTasksBar->value()) + "/" +
                                   QString::number(m_ui->totalTasksBar->maximum()));

    m_ui->currentTaskLabel->setText(tr("Current Task: "));
    m_ui->currentTaskBar->setValue(0);
}

void DoubleProgressBar::setCurrentaskAdvancement(int value)
{
    m_ui->currentTaskBar->setValue(value);
}


DoubleProgressBar::~DoubleProgressBar() = default;
