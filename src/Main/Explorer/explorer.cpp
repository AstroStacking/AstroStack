#include "explorer.h"
#include "ui_explorer.h"

#include <Common/imagedata.h>
#include <HistoAdjust/histoadjust.h>
#include <mainwindow.h>

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QFileSystemModel>
#include <QtWidgets/QMenu>

namespace astro
{
Explorer::Explorer(MainWindow* mainWindow)
    : m_ui(std::make_unique<Ui::Explorer>())
    , m_mainWindow(mainWindow)
{
    m_ui->setupUi(this);

    m_model = std::make_unique<QFileSystemModel>();
    m_model->setRootPath(QDir::rootPath());
    QStringList filters;
    filters << "*.png";
    filters << "*.jpg";
    filters << "*.tif";

    m_model->setNameFilters(filters);
    m_model->setNameFilterDisables(false);
    m_ui->treeView->setModel(m_model.get());
    m_ui->treeView->setCurrentIndex(m_model->index(QDir::homePath()));
    m_ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &Explorer::selectFile, this, &Explorer::selectImgFile);
    connect(m_ui->treeView, &QTreeView::doubleClicked, this, &Explorer::selectImg);
    connect(m_ui->treeView, &QTreeView::customContextMenuRequested, this, &Explorer::contextMenuRequested);

    restore();
}

Explorer::~Explorer()
{
    save();
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
    QString file = m_model->fileInfo(index).absoluteFilePath();
    emit selectFile(file);
}

void Explorer::selectImgFile(QString file)
{
    m_ui->display->display(file);
}

void Explorer::contextMenuRequested(QPoint pos)
{
    QModelIndex index = m_ui->treeView->indexAt(pos);

    QMenu* menu = new QMenu(this);
    QAction* processAct = new QAction(tr("Process"), this);
    processAct->setShortcuts(QKeySequence::Quit);
    processAct->setStatusTip(tr("Process"));
    connect(processAct, &QAction::triggered, this, &Explorer::openProcess);
    menu->addAction(processAct);
    menu->popup(m_ui->treeView->viewport()->mapToGlobal(pos));
}

void Explorer::openProcess()
{
    QString file = m_model->fileInfo(m_ui->treeView->currentIndex()).absoluteFilePath();
    m_mainWindow->addSubWindow(new HistoAdjust(file));
}
} // namespace astro
