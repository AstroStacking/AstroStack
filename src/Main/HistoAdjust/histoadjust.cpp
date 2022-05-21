#include "histoadjust.h"
#include "ui_histoadjust.h"

#include <imagedata.h>

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QFileSystemModel>
#include <QtWidgets/QMenu>

namespace astro
{
HistoAdjust::HistoAdjust(QWidget* parent)
    : QWidget(parent)
    , m_ui(std::make_unique<Ui::HistoAdjust>())
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

    connect(m_ui->treeView, &QTreeView::doubleClicked, this, &HistoAdjust::selectImg);
    connect(m_ui->treeView, &QTreeView::customContextMenuRequested, this, &HistoAdjust::contextMenuRequested);

    restore();
}

HistoAdjust::~HistoAdjust()
{
    save();
}

void HistoAdjust::restore()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("HistoAdjust");
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
    selectImg(m_ui->treeView->currentIndex());
    settings.endGroup();
}

void HistoAdjust::save()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("HistoAdjust");
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

void HistoAdjust::closeEvent(QCloseEvent* event)
{
    event->ignore();
}

void HistoAdjust::selectImg(const QModelIndex& index)
{
    QString file = m_model->fileInfo(index).absoluteFilePath();
    m_ui->display->display(file);
}

void HistoAdjust::contextMenuRequested(QPoint pos)
{
    QModelIndex index = m_ui->treeView->indexAt(pos);

    QMenu* menu = new QMenu(this);
    menu->addAction(new QAction("Action 1", this));
    menu->addAction(new QAction("Action 2", this));
    menu->addAction(new QAction("Action 3", this));
    menu->popup(m_ui->treeView->viewport()->mapToGlobal(pos));
}
} // namespace astro
