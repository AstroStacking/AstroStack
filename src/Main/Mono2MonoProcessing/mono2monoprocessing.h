#pragma once

#include <IO/io.h>

#include <QtCore/QFutureWatcher>
#include <QtCore/QJsonObject>
#include <QtWidgets/QWidget>

namespace Ui
{
class Mono2MonoProcessing;
}

class QFileSystemModel;
class QProgressDialog;

namespace astro
{
class Mono2MonoInterface;
class Mono2MonoInterfaceGUI;

class Mono2MonoProcessing : public QWidget
{
    Q_OBJECT

public:
    explicit Mono2MonoProcessing(QString filename, QWidget* parent = nullptr);
    ~Mono2MonoProcessing();

    void setupWorkflow(const std::vector<std::pair<Mono2MonoInterface*, QJsonObject>>& steps);

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

    std::unique_ptr<Ui::Mono2MonoProcessing> m_ui;
    AstroImage m_img;
    AstroImage m_processedImg;
    QProgressDialog* m_progressDialog{};
    QFutureWatcher<void> m_watcher;

    std::vector<Mono2MonoInterfaceGUI*> m_tasks;
};
} // namespace astro
