#include "explorer.h"
#include "ui_explorer.h"

#include <Common/imagedata.h>
#include <Mono2MonoProcessing/mono2monoprocessing.h>
#include <Mono2MonoProcessing/mono2monoworkflow.h>
#include <Multi2MultiProcessing/multi2multiprocessing.h>
#include <Multi2MultiProcessing/multi2multiworkflow.h>
#include <mainwindow.h>

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QFileSystemModel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>

namespace astro
{
Explorer::Explorer(MainWindow* mainWindow)
    : m_ui(std::make_unique<Ui::Explorer>())
    , m_mainWindow(mainWindow)
    , m_workflows(Mono2MonoWorkflow::getWorkflows(this))
    , m_multiWorkflows(Multi2MultiWorkflow::getWorkflows(this))
{
    m_ui->setupUi(this);

    m_model = std::make_unique<QFileSystemModel>();
    m_model->setRootPath(QDir::rootPath());
    QStringList filters;
    // TODO plugins to filters
    filters << "*.png";
    filters << "*.jpg";
    filters << "*.tif";
    filters << "*.tiff";

    m_model->setNameFilters(filters);
    m_model->setNameFilterDisables(false);
    m_ui->treeView->setModel(m_model.get());
    m_ui->treeView->setCurrentIndex(m_model->index(QDir::homePath()));
    m_ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &Explorer::selectFile, this, &Explorer::selectImgFile);
    connect(m_ui->treeView, &QTreeView::doubleClicked, this, &Explorer::selectImg);
    connect(m_ui->treeView, &QTreeView::customContextMenuRequested, this, &Explorer::contextMenuRequested);

    restore();
    createMenu();
    createContextMenu();
}

Explorer::~Explorer()
{
    save();
}

QString Explorer::getSelectedFile() const
{
    return m_model->fileInfo(m_ui->treeView->currentIndex()).absoluteFilePath();
}

void Explorer::addSubWindow(QWidget* widget)
{
    m_mainWindow->addSubWindow(widget);
}

void Explorer::createMenu()
{
    m_menu = m_mainWindow->menuBar()->addMenu(tr("Workflows"));
    for (auto& workflow : m_multiWorkflows)
    {
        QAction* processAct = new QAction(workflow->objectName(), this);
        processAct->setStatusTip(workflow->objectName());
        connect(processAct, &QAction::triggered, workflow.get(), &Multi2MultiWorkflow::openProcess);
        m_menu->addAction(processAct);
    }
}

void Explorer::createContextMenu()
{
    m_contextMenu = new QMenu(this);
    for (auto& workflow : m_workflows)
    {
        QAction* processAct = new QAction(workflow->objectName(), this);
        processAct->setStatusTip(workflow->objectName());
        connect(processAct, &QAction::triggered, workflow.get(), &Mono2MonoWorkflow::openProcess);
        m_contextMenu->addAction(processAct);
    }
}

void Explorer::restore()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("Explorer");
    if (!settings.contains("geometry"))
    {
        return;
    }
    restoreGeometry(settings.value("geometry").toByteArray());
    m_ui->splitter->restoreState(settings.value("splitter").toByteArray());
    m_ui->treeView->restoreGeometry(settings.value("treeGeometry").toByteArray());
    m_ui->treeView->setCurrentIndex(m_model->index(settings.value("index").toByteArray()));
    for (int i = 0; i < m_model->columnCount(); ++i)
    {
        m_ui->treeView->setColumnWidth(i, settings.value("header" + QString::number(i)).toInt());
    }
    settings.endGroup();
    selectImg(m_ui->treeView->currentIndex());
}

void Explorer::save()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("Explorer");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", m_ui->splitter->saveState());
    settings.setValue("treeGeometry", m_ui->treeView->saveGeometry());

    QModelIndex index = m_ui->treeView->currentIndex();
    QString path = m_model->fileInfo(index).absoluteFilePath();
    settings.setValue("index", path);

    for (int i = 0; i < m_model->columnCount(); ++i)
    {
        settings.setValue("header" + QString::number(i), m_ui->treeView->columnWidth(i));
    }
    settings.endGroup();
}

void Explorer::closeEvent(QCloseEvent* event)
{
    event->ignore();
}

void Explorer::selectImg(const QModelIndex& index)
{
    QFileInfo info = m_model->fileInfo(index);
    if (!info.isFile())
    {
        return;
    }
    QString file = info.absoluteFilePath();
    emit selectFile(file);
}

void Explorer::selectImgFile(QString file)
{
    m_ui->display->display(file);
}

void Explorer::contextMenuRequested(QPoint pos)
{
    m_contextMenu->popup(m_ui->treeView->viewport()->mapToGlobal(pos));
}
} // namespace astro
