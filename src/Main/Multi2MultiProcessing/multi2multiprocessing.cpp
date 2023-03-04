#include "multi2multiprocessing.h"
#include "ui_multi2multiprocessing.h"
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

namespace astro
{
Multi2MultiProcessing::Multi2MultiProcessing(QString filename, QWidget* parent)
    : QWidget(parent)
    , m_ui(std::make_unique<Ui::Multi2MultiProcessing>())
{
    m_ui->setupUi(this);
    setWindowTitle(filename);
    connect(this, &Multi2MultiProcessing::finished, this, &Multi2MultiProcessing::hasFinished);
    connect(m_ui->execute, &QPushButton::clicked, this, &Multi2MultiProcessing::run);

    restore();
}

Multi2MultiProcessing::~Multi2MultiProcessing() = default;

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

    m_progressDialog = new QProgressDialog(tr("Processing."), tr("Cancel"), 0, m_tasks.size() + 1, this);
    m_progressDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(m_progressDialog, &QProgressDialog::canceled, &m_watcher, &QFutureWatcher<void>::cancel);
    connect(&m_watcher, &QFutureWatcher<void>::progressValueChanged, m_progressDialog, &QProgressDialog::setValue);
    connect(&m_watcher, &QFutureWatcher<void>::finished, m_progressDialog, &QProgressDialog::close);

    std::vector<Multi2MultiInterfaceGUI*> activeTasks;
    for (auto task : m_tasks)
    {
        if (task->isActive())
        {
            activeTasks.push_back(task);
        }
    }

    QTemporaryFile temp;
    H5::H5File h5file;
    if (temp.open())
    {
        temp.close();
        h5file = H5::H5File(temp.fileName().toStdString(), H5F_ACC_TRUNC);
    }

    m_watcher.setFuture(QtConcurrent::run(
            [this, &h5file, activeTasks = std::move(activeTasks)](QPromise<void>& promise)
            {
                int i = 0;
                for (auto task : activeTasks)
                {
                    task->process(h5file, promise);
                    promise.setProgressValue(++i);
                    if (promise.isCanceled())
                    {
                        emit finished();
                        return;
                    }
                }
                promise.setProgressValue(++i);
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
    settings.beginGroup("Multi2MultiProcessing");
    if (!settings.contains("geometry"))
    {
        return;
    }
    restoreGeometry(settings.value("geometry").toByteArray());
    settings.endGroup();
}

void Multi2MultiProcessing::save()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("Multi2MultiProcessing");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
}
} // namespace astro
