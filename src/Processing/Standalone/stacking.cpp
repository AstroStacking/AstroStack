#include <IO/io.h>
#include <IO/itkinput.h>
#include <IO/itkoutput.h>
#include <Processing/maxstacking.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Stacking");
    QCoreApplication::setApplicationVersion("0.2.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Stacking");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("images", QCoreApplication::translate("main", "Input images."));
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output image."), "output");
    parser.addOption(outputOption);
    QCommandLineOption highdefOption("high-def", QCoreApplication::translate("main", "Save in 16bits."));
    parser.addOption(highdefOption);

    // Process the actual command line arguments given by the user
    parser.process(app);
    if (!parser.isSet(outputOption))
    {
        throw std::runtime_error("Missing output image");
    }

    QStringList filenames = parser.positionalArguments();
    std::string output = parser.value(outputOption).toStdString();
    bool highdef = parser.isSet(highdefOption);

    std::vector<astro::AstroImage> images;
    for (QString filename : filenames)
    {
        images.push_back(astro::enrichImage(filename.toStdString(), astro::io::open(filename.toStdString())));
    }

    auto img = astro::processing::maxStacking(images);

    if (highdef)
    {
        astro::io::save<uint16_t>(img.getImg(), output);
    }
    else
    {
        astro::io::save<uint8_t>(img.getImg(), output);
    }
    astro::saveEnrichedImage(output, img);

    return 0;
}
