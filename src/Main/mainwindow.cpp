#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_ui(new Ui_MainWindow)
{
    m_ui->setupUi(this);
}

MainWindow::~MainWindow() = default;
