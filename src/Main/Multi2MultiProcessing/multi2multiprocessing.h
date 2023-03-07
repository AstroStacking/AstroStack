#pragma once

#include <IO/io.h>

#include <QtCore/QFutureWatcher>
#include <QtCore/QJsonObject>
#include <QtWidgets/QWidget>

namespace Ui
{
class Multi2MultiProcessing;
}

class QFileSystemModel;
class QProgressDialog;

namespace astro
{
class Multi2MultiInterface;
class Multi2MultiInterfaceGUI;

class Multi2MultiProcessing : public QWidget
{
    Q_OBJECT

public:
    explicit Multi2MultiProcessing(QString filename, QWidget* parent = nullptr);
    ~Multi2MultiProcessing();

    void setupWorkflow(const std::vector<std::pair<Multi2MultiInterface*, QJsonObject>>& steps);
    void restore();

signals:
    void enable(bool enabled);
    void finished();

public slots:
    void run();
    void hasFinished();

private:
    void save();
    bool check();
    void execute();

    std::unique_ptr<Ui::Multi2MultiProcessing> m_ui;
    QProgressDialog* m_progressDialog{};
    QFutureWatcher<void> m_watcher;

    std::vector<Multi2MultiInterfaceGUI*> m_tasks;
};
} // namespace astro
