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
    explicit HistoAdjust(QWidget* parent = nullptr);
    ~HistoAdjust();

public slots:
    void selectImg(const QModelIndex& index);
    void contextMenuRequested(QPoint pos);

protected:
    void closeEvent(QCloseEvent* event);

private:
    void restore();
    void save();

    std::unique_ptr<Ui::HistoAdjust> m_ui;
    std::unique_ptr<QFileSystemModel> m_model;
};
} // namespace astro
