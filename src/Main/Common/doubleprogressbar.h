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
    explicit DoubleProgressBar(QString labelText, int tasks, QWidget* parent = nullptr);
    ~DoubleProgressBar() override;

signals:
    void cancel();

public slots:
    void startNewTask(int steps);
    void setCurrentaskAdvancement(int value);

private:
    std::unique_ptr<Ui::DoubleProgressBar> m_ui;
};
