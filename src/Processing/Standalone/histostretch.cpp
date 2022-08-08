#include <IO/io.h>
#include <IO/itkinput.h>
#include <IO/itkoutput.h>
#include <Processing/histostretch.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Histo Stretch");
    QCoreApplication::setApplicationVersion("0.2.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Histo Stretch");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption inputOption("input", QCoreApplication::translate("main", "Input image."), "input");
    parser.addOption(inputOption);
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output image."), "output");
    parser.addOption(outputOption);
    QCommandLineOption highdefOption("high-def", QCoreApplication::translate("main", "Save in 16bits."));
    parser.addOption(highdefOption);

    QCommandLineOption redOption("red", QCoreApplication::translate("main", "Coefficient for the red channel."), ".1");
    parser.addOption(redOption);
    QCommandLineOption greenOption("green", QCoreApplication::translate("main", "Coefficient for the green channel."), ".1");
    parser.addOption(greenOption);
    QCommandLineOption blueOption("blue", QCoreApplication::translate("main", "Coefficient for the blue channel."), ".1");
    parser.addOption(blueOption);
    QCommandLineOption relativeOption("relative", QCoreApplication::translate("main", "Use relative stretching."));
    parser.addOption(relativeOption);

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

    if (!parser.isSet(redOption))
    {
        throw std::runtime_error("Missing red argument");
    }
    if (!parser.isSet(greenOption))
    {
        throw std::runtime_error("Missing green argument");
    }
    if (!parser.isSet(blueOption))
    {
        throw std::runtime_error("Missing blue argument");
    }

    std::string input = parser.value(inputOption).toStdString();
    std::string output = parser.value(outputOption).toStdString();
    bool highdef = parser.isSet(highdefOption);

    float red = parser.value(redOption).toFloat();
    float green = parser.value(greenOption).toFloat();
    float blue = parser.value(blueOption).toFloat();
    bool relative = parser.isSet(relativeOption);

    astro::AstroImage img = astro::enrichImage(input, astro::io::open(input));

    img = astro::processing::histoStretch(img, red, green, blue, relative);

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
