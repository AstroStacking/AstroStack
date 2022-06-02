#pragma once

#include <IO/io.h>
#include <Processing/mono.h>

#include <QtCore/QFutureWatcher>
#include <QtWidgets/QWidget>

namespace Ui
{
class MonoProcessing;
}

class QFileSystemModel;
class QProgressDialog;

namespace astro
{
class MonoProcessing : public QWidget
{
    Q_OBJECT

public:
    explicit MonoProcessing(QString filename, QWidget* parent = nullptr);
    ~MonoProcessing();

    void setupWorkflow();

signals:
    void enable(bool enabled);
    void finished();

public slots:
    void run();
    void hasFinished();
    void saveAs();

private:
    void restore();
    void save();
    void loadFile(QString file);
    void processLoadFile(QString file, QPromise<void>& promise);
    bool check();
    void execute();

    std::unique_ptr<Ui::MonoProcessing> m_ui;
    ImageTypePtr m_img;
    ImageTypePtr m_processedImg;
    QProgressDialog* m_progressDialog{};
    QFutureWatcher<void> m_watcher;

    std::vector<MonoInterfaceGUI*> m_tasks;
};
} // namespace astro
