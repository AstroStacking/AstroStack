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
    explicit DoubleProgressBar(int tasks, QWidget* parent = nullptr);
    ~DoubleProgressBar() override;

public slots:
    void startNewTask();
    void setCurrentaskAdvancement(int value);

private:
    std::unique_ptr<Ui::DoubleProgressBar> m_ui;
};
