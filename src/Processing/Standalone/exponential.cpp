#include <IO/io.h>
#include <IO/itkinput.h>
#include <IO/itkoutput.h>
#include <Processing/exponential.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Exponential");
    QCoreApplication::setApplicationVersion("0.2.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Exponential");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption inputOption("input", QCoreApplication::translate("main", "Input image."), "input");
    parser.addOption(inputOption);
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output image."), "output");
    parser.addOption(outputOption);
    QCommandLineOption highdefOption("high-def", QCoreApplication::translate("main", "Save in 16bits."));
    parser.addOption(highdefOption);

    QCommandLineOption exponentialOption(
            "exponential",
            QCoreApplication::translate("main",
                                        "Exponential parameter for the transform (must be positive and less than 1)."),
            ".5");
    parser.addOption(exponentialOption);

    // Process the actual command line arguments given by the user
    parser.process(app);
    if (!parser.isSet(inputOption))
    {
        throw std::runtime_error("Missing input image");
    }
    if (!parser.isSet(outputOption))
    {
        throw std::runtime_error("Missing output image");
    }

    if (!parser.isSet(exponentialOption))
    {
        throw std::runtime_error("Missing exponential argument");
    }

    std::string input = parser.value(inputOption).toStdString();
    std::string output = parser.value(outputOption).toStdString();
    bool highdef = parser.isSet(highdefOption);

    float exponential = parser.value(exponentialOption).toFloat();

    astro::AstroImage img = astro::enrichImage(input, astro::io::open(input));

    img.setImg(astro::processing::exponential(img.getImg(), exponential));

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
