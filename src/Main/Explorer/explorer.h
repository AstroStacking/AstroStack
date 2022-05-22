#pragma once

#include <QtWidgets/QWidget>

namespace Ui
{
class Explorer;
}
class QFileSystemModel;

namespace astro
{
class MainWindow;

class Explorer : public QWidget
{
    Q_OBJECT

public:
    explicit Explorer(MainWindow* mainWindow);
    ~Explorer();

signals:
    void selectFile(QString file);

public slots:
    void selectImg(const QModelIndex& index);
    void selectImgFile(QString file);
    void contextMenuRequested(QPoint pos);
    void openProcess();

protected:
    void closeEvent(QCloseEvent* event);

private:
    void restore();
    void save();

    std::unique_ptr<Ui::Explorer> m_ui;
    std::unique_ptr<QFileSystemModel> m_model;
    MainWindow* m_mainWindow;
};
} // namespace astro
