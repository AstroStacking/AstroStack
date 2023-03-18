#include <Algos/ImageStats/image_statistics.h>
#include <IO/hdf5.h>
#include <IO/io.h>
#include <IO/itkoutput.h>
#include <Processing/stardetection.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("StarDetection");
    QCoreApplication::setApplicationVersion("0.4.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Stars Detectionv");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption inputOption("input", QCoreApplication::translate("main", "Input hdf5."), "input");
    parser.addOption(inputOption);
    QCommandLineOption inputDatasetOption("dataset", QCoreApplication::translate("main", "Input Dataset."), "input",
                                          "input");
    parser.addOption(inputDatasetOption);
    QCommandLineOption outputDatasetOption("outputDataset", QCoreApplication::translate("main", "Output Dataset."),
                                           "stars", "stars");
    parser.addOption(outputDatasetOption);
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output binary Image."), "output");
    parser.addOption(outputOption);
    QCommandLineOption thresholdOption("threshold", QCoreApplication::translate("main", "Threshold to find stars."),
                                       ".5", ".5");
    parser.addOption(thresholdOption);
    QCommandLineOption discardBiggerOption(
            "discard-bigger", QCoreApplication::translate("main", "Discard elements that are bigger than this value."),
            "100", "100");
    parser.addOption(discardBiggerOption);

    // Process the actual command line arguments given by the user
    parser.process(app);
    std::string input = parser.value(inputOption).toStdString();
    std::string inputDatasetName = parser.value(inputDatasetOption).toStdString();
    std::string output = parser.value(outputOption).toStdString();
    std::string outputDatasetName = parser.value(outputDatasetOption).toStdString();

    float threshold = parser.value(thresholdOption).toFloat();
    int32_t discardBigger = parser.value(discardBiggerOption).toInt();

    H5::H5File h5file(input, H5F_ACC_RDWR);

    H5::DataSet inputDataset = h5file.openDataSet(inputDatasetName);
    size_t needSubGroup = outputDatasetName.rfind("/");
    H5::Group group = h5file;
    if (needSubGroup != std::string::npos)
    {
        group = astro::hdf5::getOrCreateGroup(outputDatasetName.substr(0, needSubGroup), h5file);
        outputDatasetName = outputDatasetName.substr(needSubGroup + 1);
    }

    auto binaryOutput =
            astro::processing::starDetection(inputDataset, group, outputDatasetName, threshold, discardBigger);

    astro::io::save<uint8_t>(binaryOutput, output);

    return 0;
}
