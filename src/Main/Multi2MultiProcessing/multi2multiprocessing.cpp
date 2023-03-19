#include "multi2multiprocessing.h"
#include "ui_multi2multiprocessing.h"
#include <Common/doubleprogressbar.h>
#include <Common/imagedata.h>
#include <QtIO/input.h>
#include <QtIO/output.h>
#include <QtProcessing/multi2multiinterface.h>

#include <QtConcurrent/QtConcurrentRun>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QTemporaryFile>
#include <QtGui/QFileSystemModel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>

#include <numeric>

namespace astro
{
Multi2MultiProcessing::Multi2MultiProcessing(QString workflow, QWidget* parent)
    : QWidget(parent)
    , m_ui(std::make_unique<Ui::Multi2MultiProcessing>())
{
    m_ui->setupUi(this);
    setWindowTitle(workflow);
    connect(this, &Multi2MultiProcessing::finished, this, &Multi2MultiProcessing::hasFinished);
    connect(m_ui->execute, &QPushButton::clicked, this, &Multi2MultiProcessing::run);
}

Multi2MultiProcessing::~Multi2MultiProcessing()
{
    save();
}

void Multi2MultiProcessing::setupWorkflow(const std::vector<std::pair<Multi2MultiInterface*, QJsonObject>>& steps)
{
    for (const auto& step : steps)
    {
        m_tasks.push_back(step.first->generateGUI(this));
        m_tasks.back()->setup(step.second);
        m_ui->contentLayout->addWidget(m_tasks.back());
    }

    m_ui->contentLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void Multi2MultiProcessing::run()
{
    if (check())
    {
        execute();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Failed executing workflow"));
        msgBox.exec();
    }
}

bool Multi2MultiProcessing::check()
{
    for (auto task : m_tasks)
    {
        if (!task->check())
        {
            return false;
        }
    }
    return true;
}

void Multi2MultiProcessing::execute()
{
    m_ui->frame->setEnabled(false);

    m_progressDialog = new DoubleProgressBar(windowTitle(), std::accumulate(m_tasks.begin(), m_tasks.end(), 0U, [](size_t value, auto* task){
        return value + task->subTasks();}), this);
    m_progressDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(m_progressDialog, &DoubleProgressBar::cancel, &m_watcher, &QFutureWatcher<void>::cancel,
            Qt::QueuedConnection);
    connect(&m_watcher, &QFutureWatcher<void>::finished, m_progressDialog, &DoubleProgressBar::close,
            Qt::QueuedConnection);
    connect(this, &Multi2MultiProcessing::startNewTask, m_progressDialog, &DoubleProgressBar::startNewTask,
            Qt::QueuedConnection);
    connect(this, &Multi2MultiProcessing::setCurrentaskAdvancement, m_progressDialog,
            &DoubleProgressBar::setCurrentaskAdvancement, Qt::QueuedConnection);

    std::vector<Multi2MultiInterfaceGUI*> activeTasks;
    for (auto task : m_tasks)
    {
        if (task->isActive())
        {
            activeTasks.push_back(task);
        }
    }

    m_watcher.setFuture(QtConcurrent::run(
            [this, activeTasks = std::move(activeTasks)](QPromise<void>& promise)
            {
                QTemporaryFile temp;
                H5::H5File h5file;
                if (temp.open())
                {
                    temp.close();
                    h5file = H5::H5File(temp.fileName().toStdString(), H5F_ACC_TRUNC);
                }
                int i = 0;
                for (auto task : activeTasks)
                {
                    task->process(
                            h5file, [this](int steps, QString title) { emit startNewTask(steps, title); },
                            [this]() { emit setCurrentaskAdvancement(); }, promise);
                    promise.setProgressValue(++i);
                    if (promise.isCanceled())
                    {
                        emit finished();
                        return;
                    }
                }
                emit finished();
            }));
    m_progressDialog->show();
}

void Multi2MultiProcessing::hasFinished()
{
    m_ui->frame->setEnabled(true);
    setFocus();
}

void Multi2MultiProcessing::restore()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup(windowTitle());
    if (!settings.contains("geometry"))
    {
        return;
    }
    parentWidget()->restoreGeometry(settings.value("geometry").toByteArray());
    for (auto* task : m_tasks)
    {
        settings.beginGroup(task->getName());
        task->restore(settings);
        settings.endGroup();
    }
    settings.endGroup();
}

void Multi2MultiProcessing::save()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup(windowTitle());
    settings.setValue("geometry", parentWidget()->saveGeometry());
    for (auto* task : m_tasks)
    {
        settings.beginGroup(task->getName());
        task->save(settings);
        settings.endGroup();
    }
    settings.endGroup();
}
} // namespace astro
