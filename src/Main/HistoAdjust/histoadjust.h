#pragma once

#include <IO/io.h>

#include <QtCore/QFutureWatcher>
#include <QtWidgets/QWidget>

namespace Ui
{
class HistoAdjust;
}

class QFileSystemModel;
class QProgressDialog;

namespace astro
{
class MonoInterface;

class HistoAdjust : public QWidget
{
    Q_OBJECT

public:
    explicit HistoAdjust(QString filename, QWidget* parent = nullptr);
    ~HistoAdjust();

public slots:
    void run();

private:
    void restore();
    void save();
    void loadFile(QString file);
    void processLoadFile(QString file, QPromise<void>& promise);
    void setupWorkflow();

    std::unique_ptr<Ui::HistoAdjust> m_ui;
    ImageTypePtr m_img;
    QProgressDialog* m_progressDialog{};
    QFutureWatcher<void> m_watcher;

    std::vector<MonoInterface*> m_tasks;
};
} // namespace astro
