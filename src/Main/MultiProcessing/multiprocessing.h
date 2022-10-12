#pragma once

#include <IO/io.h>

#include <QtCore/QFutureWatcher>
#include <QtCore/QJsonObject>
#include <QtWidgets/QWidget>

namespace Ui
{
class MultiProcessing;
}

class QFileSystemModel;
class QProgressDialog;

namespace astro
{
class MultiInterface;
class MultiInterfaceGUI;

class MultiProcessing : public QWidget
{
    Q_OBJECT

public:
    explicit MultiProcessing(QString filename, QWidget* parent = nullptr);
    ~MultiProcessing();

    void setupWorkflow(const std::vector<std::pair<MultiInterface*, QJsonObject>>& steps);

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

    std::unique_ptr<Ui::MultiProcessing> m_ui;
    AstroImage m_img;
    AstroImage m_processedImg;
    QProgressDialog* m_progressDialog{};
    QFutureWatcher<void> m_watcher;

    std::vector<MultiInterfaceGUI*> m_tasks;
};
} // namespace astro
