#include <QApplication>

#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    astro::MainWindow window;
    window.show();
    return app.exec();
}
