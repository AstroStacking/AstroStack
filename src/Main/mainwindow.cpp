#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <explorer.h>

#include <QMdiArea>
#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_ui(new Ui_MainWindow)
{
    m_ui->setupUi(this);
    m_mdiArea = new QMdiArea();
    setCentralWidget(m_mdiArea);
    
    auto tree = m_mdiArea->addSubWindow(new Explorer());
    tree->setWindowTitle(tr("Explorer"));
}

MainWindow::~MainWindow() = default;
