#pragma once

#include <IO/io.h>
#include <Processing/mono.h>

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
class HistoAdjust : public QWidget
{
    Q_OBJECT

public:
    explicit HistoAdjust(QString filename, QWidget* parent = nullptr);
    ~HistoAdjust();

signals:
    void enable(bool enabled);
    void finished();

public slots:
    void run();
    void hasFinished();

private:
    void restore();
    void save();
    void loadFile(QString file);
    void processLoadFile(QString file, QPromise<void>& promise);
    void setupWorkflow();
    bool check();
    void execute();

    std::unique_ptr<Ui::HistoAdjust> m_ui;
    ImageTypePtr m_img;
    QProgressDialog* m_progressDialog{};
    QFutureWatcher<void> m_watcher;

    std::vector<MonoInterface::GUI*> m_tasks;
};
} // namespace astro
