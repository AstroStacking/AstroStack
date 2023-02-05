#include "multi2multiprocessing.h"
#include "ui_multi2multiprocessing.h"
#include <Common/imagedata.h>
#include <QtIO/input.h>
#include <QtIO/output.h>
#include <QtProcessing/multi2multiinterface.h>

#include <QtConcurrent/QtConcurrentRun>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QFileSystemModel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>

namespace astro
{
MultiProcessing::MultiProcessing(QString filename, QWidget* parent)
    : QWidget(parent)
    , m_ui(std::make_unique<Ui::MultiProcessing>())
{
    m_ui->setupUi(this);
    setWindowTitle(filename);
    connect(this, &MultiProcessing::finished, this, &MultiProcessing::hasFinished);
    connect(m_ui->execute, &QPushButton::clicked, this, &MultiProcessing::run);
    connect(m_ui->saveAs, &QPushButton::clicked, this, &MultiProcessing::saveAs);

    restore();
    loadFile(filename);
}

MultiProcessing::~MultiProcessing()
{
    save();
}

void MultiProcessing::loadFile(QString file)
{
    m_progressDialog = new QProgressDialog(tr("Loading in progress."), tr("Cancel"), 0, 3, this);
    m_progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_progressDialog, &QProgressDialog::canceled, &m_watcher, &QFutureWatcher<void>::cancel);
    connect(&m_watcher, &QFutureWatcher<void>::progressValueChanged, m_progressDialog, &QProgressDialog::setValue);
    connect(&m_watcher, &QFutureWatcher<void>::finished, m_progressDialog, &QProgressDialog::close);

    m_watcher.setFuture(QtConcurrent::run([=](QPromise<void>& promise) { processLoadFile(file, promise); }));
    m_progressDialog->show();
}

void MultiProcessing::processLoadFile(QString file, QPromise<void>& promise)
{
    m_processedImg = m_img = InputInterface::loadImg(file, this);
    promise.setProgressValue(1);
    if (m_img.isValid())
    {
        m_ui->input->handleItem(m_img);
        promise.setProgressValue(2);
        m_ui->output->handleItem(m_img);
        promise.setProgressValue(3);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Could not load image ") + file + ".");
        msgBox.exec();
    }
}

void MultiProcessing::setupWorkflow(const std::vector<std::pair<MultiInterface*, QJsonObject>>& steps)
{
    for (const auto& step : steps)
    {
        m_tasks.push_back(step.first->generateGUI(this));
        m_tasks.back()->setup(step.second);
        m_ui->contentLayout->addWidget(m_tasks.back());
    }

    m_ui->contentLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void MultiProcessing::run()
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

bool MultiProcessing::check()
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

void MultiProcessing::execute()
{
    m_ui->frame->setEnabled(false);

    m_progressDialog = new QProgressDialog(tr("Processing."), tr("Cancel"), 0, m_tasks.size() + 1, this);
    m_progressDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(m_progressDialog, &QProgressDialog::canceled, &m_watcher, &QFutureWatcher<void>::cancel);
    connect(&m_watcher, &QFutureWatcher<void>::progressValueChanged, m_progressDialog, &QProgressDialog::setValue);
    connect(&m_watcher, &QFutureWatcher<void>::finished, m_progressDialog, &QProgressDialog::close);

    std::vector<MultiInterfaceGUI*> activeTasks;
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
                int i = 0;
                AstroImage img = m_img;
                for (auto task : activeTasks)
                {
                    img = task->process(img, promise);
                    promise.setProgressValue(++i);
                    if (promise.isCanceled())
                    {
                        emit finished();
                        return;
                    }
                }
                m_processedImg = img;
                m_ui->output->handleItem(m_processedImg);
                promise.setProgressValue(++i);
                emit finished();
            }));
    m_progressDialog->show();
}

void MultiProcessing::hasFinished()
{
    m_ui->frame->setEnabled(true);
    setFocus();
}

void MultiProcessing::saveAs()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("MultiProcessing");
    QString filename = settings.value("saveAs", "").toString();

    filename = QFileDialog::getSaveFileName(this, tr("Save output"), filename);
    if (!filename.isEmpty())
    {
        settings.setValue("saveAs", filename);
        OutputInterface::saveImg(m_processedImg, filename, this);
    }
}

void MultiProcessing::restore()
{
    m_ui->input->restore("ImageDisplayInput");
    m_ui->output->restore("ImageDisplayOutput");

    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("MultiProcessing");
    if (!settings.contains("geometry"))
    {
        return;
    }
    restoreGeometry(settings.value("geometry").toByteArray());
    m_ui->splitter->restoreState(settings.value("splitter").toByteArray());
    settings.endGroup();
}

void MultiProcessing::save()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("MultiProcessing");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", m_ui->splitter->saveState());
    settings.endGroup();
}
} // namespace astro
