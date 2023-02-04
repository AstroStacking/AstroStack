#include <IO/io.h>
#include <IO/itkinput.h>
#include <IO/itkoutput.h>
#include <Processing/rldeconvolution.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Deconvolution");
    QCoreApplication::setApplicationVersion("0.2.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Deconvolution");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption inputOption("input", QCoreApplication::translate("main", "Input image."), "input");
    parser.addOption(inputOption);
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output image."), "output");
    parser.addOption(outputOption);
    QCommandLineOption highdefOption("high-def", QCoreApplication::translate("main", "Save in 16bits."));
    parser.addOption(highdefOption);
    QCommandLineOption sizeOption("filter-size",
                                  QCoreApplication::translate("main", "Filter size to use for the Gaussian kernel."),
                                  "4", "4");
    parser.addOption(sizeOption);
    QCommandLineOption sigmaOption("sigma", QCoreApplication::translate("main", "Sigma of the Gaussian kernel."), "2",
                                   "2");
    parser.addOption(sigmaOption);

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

    if (!parser.isSet(sizeOption))
    {
        throw std::runtime_error("Missing filter size argument");
    }
    if (!parser.isSet(sigmaOption))
    {
        throw std::runtime_error("Missing sigma argument");
    }

    std::string input = parser.value(inputOption).toStdString();
    std::string output = parser.value(outputOption).toStdString();
    bool highdef = parser.isSet(highdefOption);

    int filterSize = parser.value(sizeOption).toInt();
    float sigma = parser.value(sigmaOption).toFloat();

    astro::AstroImage img = astro::enrichImage(input, astro::io::open(input));

    img.setImg(astro::processing::RLDeconvolution(img.getImg(), filterSize, sigma));

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
