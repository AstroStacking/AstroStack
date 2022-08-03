#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("ChromaSmoothing");
    QCoreApplication::setApplicationVersion("0.2.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Chromatic Smoothing");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source image."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination image."));

    QCommandLineOption varianceOption(
            "variance", QCoreApplication::translate("main", "Variance for the Hue and Saturation smoothing."));
    parser.addOption(varianceOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    return 0;
}
