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
class Mono2MonoWorkflow;
class Multi2MultiWorkflow;

class Explorer : public QWidget
{
    Q_OBJECT

public:
    explicit Explorer(MainWindow* mainWindow);
    ~Explorer();

    QString getSelectedFile() const;
    void addSubWindow(QWidget* widget);

signals:
    void selectFile(QString file);

public slots:
    void selectImg(const QModelIndex& index);
    void selectImgFile(QString file);
    void contextMenuRequested(QPoint pos);

protected:
    void closeEvent(QCloseEvent* event);

private:
    void createMenu();
    void createContextMenu();
    void restore();
    void save();

    std::unique_ptr<Ui::Explorer> m_ui;
    std::unique_ptr<QFileSystemModel> m_model;
    MainWindow* m_mainWindow{};
    QMenu* m_menu{};
    QMenu* m_contextMenu{};
    std::vector<std::unique_ptr<Mono2MonoWorkflow>> m_workflows;
    std::vector<std::unique_ptr<Multi2MultiWorkflow>> m_multiWorkflows;
};
} // namespace astro
