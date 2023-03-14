#include "doubleprogressbar.h"
#include "ui_doubleprogressbar.h"

DoubleProgressBar::DoubleProgressBar(QString labelText, int tasks, QWidget* parent)
    : QDialog(parent)
    , m_ui(std::make_unique<Ui::DoubleProgressBar>())
{
    setWindowTitle(labelText);
    m_ui->setupUi(this);
    m_ui->totalTasksBar->setValue(0);
    m_ui->totalTasksBar->setMaximum(tasks);
    m_ui->totalTasksLabel->setText(tr("Task ") + QString::number(m_ui->totalTasksBar->value()) + "/" +
                                   QString::number(tasks));

    connect(m_ui->cancel, &QPushButton::clicked, this, &DoubleProgressBar::cancel);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setModal(true);
}

void DoubleProgressBar::startNewTask(int steps)
{
    m_ui->totalTasksLabel->setText(tr("Task ") + QString::number(m_ui->totalTasksBar->value() + 1) + "/" +
                                   QString::number(m_ui->totalTasksBar->maximum()));
    m_ui->totalTasksBar->setValue(m_ui->totalTasksBar->value() + 1);

    m_ui->currentTaskBar->setMaximum(steps);
    m_ui->currentTaskLabel->setText(tr("Current Task: "));
    m_ui->currentTaskBar->setValue(0);
}

void DoubleProgressBar::setCurrentaskAdvancement(int value)
{
    m_ui->currentTaskLabel->setText(tr("Current Task: ") + QString::number(m_ui->currentTaskBar->value() + 1) + "/" +
                                    QString::number(m_ui->currentTaskBar->maximum()));
    m_ui->currentTaskBar->setValue(value);
}


DoubleProgressBar::~DoubleProgressBar() = default;
