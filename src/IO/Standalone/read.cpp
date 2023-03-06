#include <IO/hdf5.h>
#include <IO/io.h>
#include <IO/itkinput.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Read");
    QCoreApplication::setApplicationVersion("0.4.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Read");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("images", QCoreApplication::translate("main", "Input images."));
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output hdf5."), "output");
    parser.addOption(outputOption);
    QCommandLineOption outputDatasetOption("dataset", QCoreApplication::translate("main", "Dataset hdf5."), "inputs",
                                           "inputs");
    parser.addOption(outputDatasetOption);

    // Process the actual command line arguments given by the user
    parser.process(app);
    if (!parser.isSet(outputOption))
    {
        throw std::runtime_error("Missing output hdf5");
    }

    QStringList filenames = parser.positionalArguments();
    std::string output = parser.value(outputOption).toStdString();
    std::string inputsDatasetName = parser.value(outputDatasetOption).toStdString();

    H5::H5File h5file(output, H5F_ACC_TRUNC);

    astro::AstroImage refImg =
            astro::enrichImage(filenames.front().toStdString(), astro::io::open(filenames.front().toStdString()));
    auto size = refImg.getImg()->GetRequestedRegion().GetSize();

    std::vector<std::string> transformedFilenames;
    for (QString filename : filenames)
    {
        transformedFilenames.push_back(filename.toStdString());
    }

    astro::hdf5::readTo(transformedFilenames, size, h5file, inputsDatasetName);

    return 0;
}
