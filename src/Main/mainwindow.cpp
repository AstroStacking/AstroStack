#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <explorer.h>

#include <QtCore/QSettings>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMdiSubWindow>

namespace astro
{
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(std::make_unique<Ui_MainWindow>())
{
    m_ui->setupUi(this);
    m_mdiArea = new QMdiArea();
    setCentralWidget(m_mdiArea);

    m_treeWindow = m_mdiArea->addSubWindow(new Explorer());
    m_treeWindow->setWindowTitle(tr("Explorer"));

    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("Main");
    if (!settings.contains("geometry"))
    {
        return;
    }
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    m_treeWindow->restoreGeometry(settings.value("explorerGeometry").toByteArray());
    m_treeWindow->move(settings.value("explorerPosition").toPoint());
    settings.endGroup();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent* event)
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("Main");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("explorerGeometry", m_treeWindow->saveGeometry());
    settings.setValue("explorerPosition", m_treeWindow->pos());
    settings.endGroup();
    QMainWindow::closeEvent(event);
}
} // namespace astro
