#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <Explorer/explorer.h>

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

    m_treeWindow = m_mdiArea->addSubWindow(new Explorer(this));
    m_treeWindow->setWindowTitle(tr("Explorer"));

    createActions();
    createMenus();

    restore();
}

MainWindow::~MainWindow() = default;

void MainWindow::addSubWindow(QWidget* window)
{
    m_mdiArea->addSubWindow(window);
    window->show();
}

void MainWindow::createActions()
{
    m_exitAct = new QAction(tr("E&xit"), this);
    m_exitAct->setShortcuts(QKeySequence::Quit);
    m_exitAct->setStatusTip(tr("Exit the application"));
    connect(m_exitAct, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_exitAct);
}

void MainWindow::restore()
{
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
