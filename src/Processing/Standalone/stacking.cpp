#include <IO/hdf5.h>
#include <IO/io.h>
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
    QCommandLineOption inputOption("input", QCoreApplication::translate("main", "Input hdf5."), "input");
    parser.addOption(inputOption);
    QCommandLineOption inputDatasetOption("dataset", QCoreApplication::translate("main", "Input Dataset."), "inputs",
                                          "inputs");
    parser.addOption(inputDatasetOption);
    QCommandLineOption outputDatasetOption("outputDataset", QCoreApplication::translate("main", "Output Dataset."),
                                           "stack", "stack");
    parser.addOption(outputDatasetOption);
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
    std::string input = parser.value(inputOption).toStdString();
    std::string inputsDatasetName = parser.value(inputDatasetOption).toStdString();
    std::string output = parser.value(outputOption).toStdString();
    std::string outputDatasetName = parser.value(outputDatasetOption).toStdString();
    bool highdef = parser.isSet(highdefOption);

    H5::H5File h5file(input, H5F_ACC_RDWR);

    H5::DataSet inputsDataset = h5file.openDataSet(inputsDatasetName);

    H5::DataSpace dataspace = inputsDataset.getSpace();
    int ndims = dataspace.getSimpleExtentNdims();
    if (ndims != 4)
    {
        throw std::runtime_error("Wrong number of dimensions");
    }
    hsize_t dims[4];
    ndims = dataspace.getSimpleExtentDims(dims, nullptr);
    if (dims[3] != astro::PixelDimension)
    {
        throw std::runtime_error("Wrong pixel dimension");
    }

    hsize_t outputImgDim[3]{dims[1], dims[2], astro::PixelDimension};
    H5::DataSpace outputSpace(3, outputImgDim);
    H5::DataSet outputDataset;

    size_t needSubGroup = outputDatasetName.rfind("/");
    if (needSubGroup == std::string::npos)
    {
        outputDataset = h5file.createDataSet(outputDatasetName, H5::PredType::NATIVE_FLOAT, outputSpace);
    }
    else
    {
        H5::Group group;
        try
        {
            group = h5file.openGroup(outputDatasetName.substr(0, needSubGroup));
        }
        catch (const H5::FileIException&)
        {
            group = h5file.createGroup(outputDatasetName.substr(0, needSubGroup));
        }
        outputDataset = group.createDataSet(outputDatasetName.substr(needSubGroup + 1), H5::PredType::NATIVE_FLOAT,
                                            outputSpace);
    }

    astro::processing::maxStacking(inputsDataset, outputDataset);
    astro::ImageTypePtr result = astro::hdf5::extractFrom(outputDataset);

    if (highdef)
    {
        astro::io::save<uint16_t>(result, output);
    }
    else
    {
        astro::io::save<uint8_t>(result, output);
    }
    //astro::saveEnrichedImage(output, refImg);

    return 0;
}
