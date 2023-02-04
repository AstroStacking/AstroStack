#include <IO/io.h>
#include <IO/itkinput.h>
#include <IO/itkoutput.h>
#include <Processing/chromasmoothing.h>

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
    QCommandLineOption inputOption("input", QCoreApplication::translate("main", "Input image."), "input");
    parser.addOption(inputOption);
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output image."), "output");
    parser.addOption(outputOption);
    QCommandLineOption highdefOption("high-def", QCoreApplication::translate("main", "Save in 16bits."));
    parser.addOption(highdefOption);
    QCommandLineOption varianceOption(
            "variance", QCoreApplication::translate("main", "Variance for the Hue and Saturation smoothing."), "2.0",
            "2.0");
    parser.addOption(varianceOption);

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

    if (!parser.isSet(varianceOption))
    {
        throw std::runtime_error("Missing variance argument");
    }

    std::string input = parser.value(inputOption).toStdString();
    std::string output = parser.value(outputOption).toStdString();
    bool highdef = parser.isSet(highdefOption);

    float variance = parser.value(varianceOption).toFloat();

    astro::AstroImage img = astro::enrichImage(input, astro::io::open(input));
    img.setImg(astro::processing::chromaSmoothing(img.getImg(), variance));

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
