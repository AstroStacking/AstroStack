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
    QCoreApplication::setApplicationName("Register");
    QCoreApplication::setApplicationVersion("0.2.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Register");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption input1Option("input1", QCoreApplication::translate("main", "Input image."), "input1");
    parser.addOption(input1Option);
    QCommandLineOption input2Option("input2", QCoreApplication::translate("main", "Input image on which to register."), "input2");
    parser.addOption(input2Option);
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output image."), "output");
    parser.addOption(outputOption);
    QCommandLineOption highdefOption("high-def", QCoreApplication::translate("main", "Save in 16bits."));
    parser.addOption(highdefOption);

    // Process the actual command line arguments given by the user
    parser.process(app);
    if (!parser.isSet(input1Option))
    {
        throw std::runtime_error("Missing input1 image");
    }
    if (!parser.isSet(input2Option))
    {
        throw std::runtime_error("Missing input2 image");
    }
    if (!parser.isSet(outputOption))
    {
        throw std::runtime_error("Missing output image");
    }

    std::string input1 = parser.value(input1Option).toStdString();
    std::string input2 = parser.value(input2Option).toStdString();
    std::string output = parser.value(outputOption).toStdString();
    bool highdef = parser.isSet(highdefOption);

    astro::AstroImage img = astro::enrichImage(input1, astro::io::open(input1));

    // segment image 1
    // segment image 2
    // Find match 1 on 2
    // Register 1 on 2
    
    
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
