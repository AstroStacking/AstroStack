#include <IO/io.h>
#include <IO/itkinput.h>
#include <IO/itkoutput.h>
#include <Processing/lightpollution.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Light Pollution");
    QCoreApplication::setApplicationVersion("0.2.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Light Pollution");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption inputOption("input", QCoreApplication::translate("main", "Input image."));
    parser.addOption(inputOption);
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output image."));
    parser.addOption(outputOption);
    QCommandLineOption highdefOption("high-def", QCoreApplication::translate("main", "Save in 16bits."));
    parser.addOption(highdefOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    std::string input = parser.value(inputOption).toStdString();
    std::string output = parser.value(outputOption).toStdString();
    bool highdef = parser.isSet(highdefOption);

    astro::AstroImage img = astro::enrichImage(input, astro::io::open(input));

    img = astro::processing::lightPollution(img);
    
    if(highdef)
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
