#include <IO/hdf5.h>
#include <IO/io.h>
#include <IO/itkinput.h>
#include <IO/itkoutput.h>
#include <Processing/graphmatching.h>
#include <Processing/grey.h>
#include <Processing/register.h>
#include <Processing/stardetection.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <QtCore/qtemporaryfile.h>

#include <H5Cpp.h>

std::vector<std::pair<double, double>> read(H5::DataSet dataset)
{
    H5::DataSpace dataspace = dataset.getSpace();
    int ndims = dataspace.getSimpleExtentNdims();
    if (ndims != 2)
    {
        throw std::runtime_error("Wrong number of dimensions");
    }
    hsize_t dims[2];
    ndims = dataspace.getSimpleExtentDims(dims, nullptr);
    if (dims[1] <= 2)
    {
        throw std::runtime_error("Wrong number of columns " + std::to_string(dims[1]));
    }

    std::vector<double> buffer(dims[0] * dims[1]);
    dataset.read(buffer.data(), H5::PredType::NATIVE_DOUBLE, dataspace, dataspace);

    std::vector<std::pair<double, double>> data(2 * dims[0]);
    for (size_t i = 0; i < dims[0]; ++i)
    {
        data[i].first = buffer[i * dims[1]];
        data[i].second = buffer[i * dims[1] + 1];
    }

    return data;
}


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
    QCommandLineOption input2Option("input2", QCoreApplication::translate("main", "Input image on which to register."),
                                    "input2");
    parser.addOption(input2Option);
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output image."), "output");
    parser.addOption(outputOption);
    QCommandLineOption highdefOption("high-def", QCoreApplication::translate("main", "Save in 16bits."));
    parser.addOption(highdefOption);
    QCommandLineOption minStarsOption("min-stars", QCoreApplication::translate("main", "Minimum number of stars."),
                                      "main", "80");
    parser.addOption(minStarsOption);
    QCommandLineOption maxStarsOption("max-stars", QCoreApplication::translate("main", "Maximum number of stars."),
                                      "main", "120");
    parser.addOption(maxStarsOption);
    QCommandLineOption fullGraphOption("full-graph", QCoreApplication::translate("main", "Full graph matching size."),
                                       "main", "5");
    parser.addOption(fullGraphOption);
    QCommandLineOption maxRatioOption(
            "max-ratio", QCoreApplication::translate("main", "Max allowed ratio during matching."), "main", ".01");
    parser.addOption(maxRatioOption);

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

    int32_t minStars = parser.value(minStarsOption).toInt();
    int32_t maxStars = parser.value(maxStarsOption).toInt();
    int fullGraph = parser.value(fullGraphOption).toInt();
    double maxRatio = parser.value(maxRatioOption).toDouble();

    bool highdef = parser.isSet(highdefOption);

    astro::AstroImage refImg = astro::enrichImage(input1, astro::io::open(input1));
    astro::AstroImage targetImg = astro::enrichImage(input2, astro::io::open(input2));
    auto size = refImg.getImg()->GetRequestedRegion().GetSize();

    QTemporaryFile temp;
    H5::H5File h5file;
    if (temp.open())
    {
        temp.close();
        h5file = H5::H5File(temp.fileName().toStdString(), H5F_ACC_TRUNC);
    }

    H5::DataSet inputs = astro::hdf5::readTo({input1, input2}, size, h5file, "inputs");

    astro::ScalarImageTypePtr refGrey = astro::processing::grey(inputs, 0, h5file, "refGrey");
    astro::ScalarImageTypePtr targetGrey = astro::processing::grey(inputs, 1, h5file, "targetGrey");

    astro::processing::starDetection(h5file.openDataSet("refGrey"), h5file, "refStar", minStars, maxStars);
    astro::processing::starDetection(h5file.openDataSet("targetGrey"), h5file, "targetStar", minStars, maxStars);

    std::vector<std::pair<double, double>> graph1 = read(h5file.openDataSet("refStar"));
    std::vector<std::pair<double, double>> graph2 = read(h5file.openDataSet("targetStar"));

    std::vector<std::pair<size_t, size_t>> matches =
            astro::processing::graphmatching(graph1, graph2, fullGraph, maxRatio);

    std::vector<std::pair<double, double>> refStars;
    std::vector<std::pair<double, double>> targetStars;
    for (auto pair : matches)
    {
        refStars.push_back(graph1[pair.first]);
        targetStars.push_back(graph2[pair.second]);
    }

    astro::ImageTypePtr outputImg =
            astro::processing::registerImages(refImg.getImg(), targetImg.getImg(), refStars, targetStars);


    if (highdef)
    {
        astro::io::save<uint16_t>(outputImg, output);
    }
    else
    {
        astro::io::save<uint8_t>(outputImg, output);
    }
    astro::saveEnrichedImage(output, refImg);

    return 0;
}
