#include "histoadjust.h"
#include "ui_histoadjust.h"

#include <Common/imagedata.h>
#include <IO/input.h>
#include <Processing/exponential.h> // temporary
#include <Processing/mono.h>

#include <QtConcurrent/QtConcurrentRun>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QFileSystemModel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>

namespace astro
{
HistoAdjust::HistoAdjust(QString filename, QWidget* parent)
    : QWidget(parent)
    , m_ui(std::make_unique<Ui::HistoAdjust>())
{
    m_ui->setupUi(this);
    setWindowTitle(filename);
    connect(m_ui->execute, &QPushButton::clicked, this, &HistoAdjust::run);
    connect(this, &HistoAdjust::finished, this, &HistoAdjust::hasFinished);

    restore();
    loadFile(filename);

    setupWorkflow();
}

HistoAdjust::~HistoAdjust()
{
    save();
}

void HistoAdjust::loadFile(QString file)
{
    m_progressDialog = new QProgressDialog(tr("Loading in progress."), tr("Cancel"), 0, 3, this);
    m_progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_progressDialog, &QProgressDialog::canceled, &m_watcher, &QFutureWatcher<void>::cancel);
    connect(&m_watcher, &QFutureWatcher<void>::progressValueChanged, m_progressDialog, &QProgressDialog::setValue);
    connect(&m_watcher, &QFutureWatcher<void>::finished, m_progressDialog, &QProgressDialog::close);

    m_watcher.setFuture(QtConcurrent::run([=](QPromise<void>& promise) { processLoadFile(file, promise); }));
    m_progressDialog->show();
}

void HistoAdjust::processLoadFile(QString file, QPromise<void>& promise)
{
    m_img = InputInterface::loadImg(file, this);
    promise.setProgressValue(1);
    if (m_img)
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

void HistoAdjust::setupWorkflow()
{
    m_tasks.push_back(Exponential().generateGUI(this));
    m_ui->contentLayout->addWidget(m_tasks.back());

    m_ui->contentLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void HistoAdjust::run()
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

bool HistoAdjust::check()
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

void HistoAdjust::execute()
{
    m_ui->frame->setEnabled(false);

    m_progressDialog = new QProgressDialog(tr("Processing."), tr("Cancel"), 0, m_tasks.size() + 1, this);
    m_progressDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(m_progressDialog, &QProgressDialog::canceled, &m_watcher, &QFutureWatcher<void>::cancel);
    connect(&m_watcher, &QFutureWatcher<void>::progressValueChanged, m_progressDialog, &QProgressDialog::setValue);
    connect(&m_watcher, &QFutureWatcher<void>::finished, m_progressDialog, &QProgressDialog::close);

    std::vector<MonoInterface::GUI*> activeTasks;
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
                ImageTypePtr img = m_img;
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
                m_ui->output->handleItem(img);
                promise.setProgressValue(++i);
                emit finished();
            }));
    m_progressDialog->show();
}

void HistoAdjust::hasFinished()
{
    m_ui->frame->setEnabled(true);
    setFocus();
}

void HistoAdjust::restore()
{
    m_ui->input->restore("ImageDisplayInput");
    m_ui->output->restore("ImageDisplayOutput");

    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("HistoAdjust");
    if (!settings.contains("geometry"))
    {
        return;
    }
    restoreGeometry(settings.value("geometry").toByteArray());
    m_ui->splitter->restoreState(settings.value("splitter").toByteArray());
    settings.endGroup();
}

void HistoAdjust::save()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("HistoAdjust");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", m_ui->splitter->saveState());
    settings.endGroup();
}
} // namespace astro
