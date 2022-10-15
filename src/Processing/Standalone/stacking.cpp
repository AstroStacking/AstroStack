#include <IO/hdf5.h>
#include <IO/io.h>
#include <IO/itkinput.h>
#include <IO/itkoutput.h>
#include <Processing/maxstacking.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <QtCore/QTemporaryFile>

namespace
{
std::string inputsDatasetName{"inputs"};
std::string outputDatasetName{"output"};
} // namespace

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

    QTemporaryFile file;
    file.close();

    H5::H5File h5file(file.fileName().toStdString().c_str(), H5F_ACC_TRUNC);

    astro::AstroImage refImg =
            astro::enrichImage(filenames.front().toStdString(), astro::io::open(filenames.front().toStdString()));
    auto size = refImg.getImg()->GetRequestedRegion().GetSize();

    std::vector<std::string> transformedFilenames;
    for (QString filename : filenames)
    {
        transformedFilenames.push_back(filename.toStdString());
    }

    H5::DataSet inputsDataset = astro::readTo(transformedFilenames, size, h5file, inputsDatasetName);

    hsize_t outputImgDim[3]{size.at(0), size.at(1), astro::PixelDimension};
    H5::DataSpace outputSpace(3, outputImgDim);
    H5::DataSet outputDataset = h5file.createDataSet(outputDatasetName, H5::PredType::NATIVE_FLOAT, outputSpace);

    astro::processing::maxStacking(inputsDataset, outputDataset);

    refImg.setImg(astro::extractFrom(outputDataset));

    if (highdef)
    {
        astro::io::save<uint16_t>(refImg.getImg(), output);
    }
    else
    {
        astro::io::save<uint8_t>(refImg.getImg(), output);
    }
    astro::saveEnrichedImage(output, refImg);

    return 0;
}
