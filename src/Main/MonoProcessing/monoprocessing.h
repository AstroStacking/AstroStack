#pragma once

#include <IO/io.h>

#include <QtCore/QFutureWatcher>
#include <QtCore/QJsonObject>
#include <QtWidgets/QWidget>

namespace Ui
{
class MonoProcessing;
}

class QFileSystemModel;
class QProgressDialog;

namespace astro
{
class MonoInterface;
class MonoInterfaceGUI;

class MonoProcessing : public QWidget
{
    Q_OBJECT

public:
    explicit MonoProcessing(QString filename, QWidget* parent = nullptr);
    ~MonoProcessing();

    void setupWorkflow(const std::vector<std::pair<MonoInterface*, QJsonObject>>& steps);

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
    AstroImage m_img;
    AstroImage m_processedImg;
    QProgressDialog* m_progressDialog{};
    QFutureWatcher<void> m_watcher;

    std::vector<MonoInterfaceGUI*> m_tasks;
};
} // namespace astro
