#include "explorer.h"
#include "ui_explorer.h"

#include <QDir>
#include <QFileSystemModel>
#include <QSettings>

#include <iostream>

Explorer::Explorer(QWidget* parent)
    : QWidget(parent), m_ui(std::make_unique<Ui::Explorer>())
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

    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("Explorer");
    if(!settings.contains("geometry"))
    {
        return;
    }
    restoreGeometry(settings.value("geometry").toByteArray());
    m_ui->treeView->restoreGeometry(settings.value("treeGeometry").toByteArray());
    m_ui->treeView->setCurrentIndex(m_model->index(settings.value("index").toByteArray()));
    for(int i = 0; i < m_model->columnCount(); ++i)
    {
        m_ui->treeView->setColumnWidth(i, settings.value("header" + QString::number(i)).toInt());
    }
    settings.endGroup();
}

Explorer::~Explorer()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("Explorer");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("treeGeometry", m_ui->treeView->saveGeometry());

    QModelIndex index = m_ui->treeView->currentIndex();
    QString path(index.data().toString());
    while(index.parent() != QModelIndex())
    {
        index = index.parent();
        path = index.data().toString() + "/" + path;
    }
    settings.setValue("index", path);
    
    for(int i = 0; i < m_model->columnCount(); ++i)
    {
        settings.setValue("header" + QString::number(i), m_ui->treeView->columnWidth(i));
    }
    settings.endGroup();
}
