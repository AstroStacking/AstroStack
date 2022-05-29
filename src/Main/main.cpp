#include "mainwindow.h"

#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QTranslator appTranslator;
    if (appTranslator.load(QLocale::system(), u"astro"_qs, u"_"_qs, u":/i18n"_qs))
    {
        app.installTranslator(&appTranslator);
    }

    astro::MainWindow window;
    window.show();
    return app.exec();
}
