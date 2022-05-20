#pragma once

#include <QtWidgets/QWidget>

namespace Ui
{
class Explorer;
}
class QFileSystemModel;

namespace astro
{
class Explorer : public QWidget
{
    Q_OBJECT

public:
    explicit Explorer(QWidget* parent = nullptr);
    ~Explorer();

public slots:
    void selectImg(const QModelIndex& index);

private:
    std::unique_ptr<Ui::Explorer> m_ui;
    std::unique_ptr<QFileSystemModel> m_model;
};
} // namespace astro
