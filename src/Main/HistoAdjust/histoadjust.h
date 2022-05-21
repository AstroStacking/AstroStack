#pragma once

#include <QtWidgets/QWidget>

namespace Ui
{
class HistoAdjust;
}
class QFileSystemModel;

namespace astro
{
class HistoAdjust : public QWidget
{
    Q_OBJECT

public:
    explicit HistoAdjust(QString filename, QWidget* parent);
    ~HistoAdjust();

public slots:

private:
    void restore();
    void save();

    std::unique_ptr<Ui::HistoAdjust> m_ui;
};
} // namespace astro
