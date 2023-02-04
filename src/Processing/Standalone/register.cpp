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

    std::vector<std::pair<double, double>> data(dims[0]);
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
    QCommandLineOption referenceOption("reference", QCoreApplication::translate("main", "Reference image."),
                                       "reference");
    parser.addOption(referenceOption);
    QCommandLineOption targetOption("target", QCoreApplication::translate("main", "Image to register."), "main");
    parser.addOption(targetOption);
    QCommandLineOption outputOption("output", QCoreApplication::translate("main", "Output image."), "output");
    parser.addOption(outputOption);
    QCommandLineOption highdefOption("high-def", QCoreApplication::translate("main", "Save in 16bits."));
    parser.addOption(highdefOption);
    QCommandLineOption minStarsOption("min-stars", QCoreApplication::translate("main", "Minimum number of stars."),
                                      "80", "80");
    parser.addOption(minStarsOption);
    QCommandLineOption maxStarsOption("max-stars", QCoreApplication::translate("main", "Maximum number of stars."),
                                      "120", "120");
    parser.addOption(maxStarsOption);
    QCommandLineOption fullGraphOption("full-graph", QCoreApplication::translate("main", "Full graph matching size."),
                                       "5", "5");
    parser.addOption(fullGraphOption);
    QCommandLineOption maxRatioOption(
            "max-ratio", QCoreApplication::translate("main", "Max allowed ratio during matching."), ".01", ".01");
    parser.addOption(maxRatioOption);

    // Process the actual command line arguments given by the user
    parser.process(app);
    if (!parser.isSet(referenceOption))
    {
        throw std::runtime_error("Missing reference image");
    }
    if (!parser.isSet(targetOption))
    {
        throw std::runtime_error("Missing target image");
    }
    if (!parser.isSet(outputOption))
    {
        throw std::runtime_error("Missing output image");
    }

    std::string reference = parser.value(referenceOption).toStdString();
    std::string target = parser.value(targetOption).toStdString();
    std::string output = parser.value(outputOption).toStdString();

    int32_t minStars = parser.value(minStarsOption).toInt();
    int32_t maxStars = parser.value(maxStarsOption).toInt();
    int fullGraph = parser.value(fullGraphOption).toInt();
    double maxRatio = parser.value(maxRatioOption).toDouble();

    bool highdef = parser.isSet(highdefOption);

    astro::AstroImage refImg = astro::enrichImage(reference, astro::io::open(reference));
    astro::AstroImage targetImg = astro::enrichImage(target, astro::io::open(target));
    auto size = refImg.getImg()->GetRequestedRegion().GetSize();

    QTemporaryFile temp;
    H5::H5File h5file;
    if (temp.open())
    {
        temp.close();
        h5file = H5::H5File(temp.fileName().toStdString(), H5F_ACC_TRUNC);
    }

    H5::DataSet inputs = astro::hdf5::readTo({reference, target}, size, h5file, "inputs");

    astro::ScalarImageTypePtr refGrey = astro::processing::grey(inputs, 0, h5file, "refGrey");
    astro::ScalarImageTypePtr targetGrey = astro::processing::grey(inputs, 1, h5file, "targetGrey");

    astro::processing::starDetection(h5file.openDataSet("refGrey"), h5file, "refStar", minStars, maxStars);
    astro::processing::starDetection(h5file.openDataSet("targetGrey"), h5file, "targetStar", minStars, maxStars);

    std::vector<std::pair<double, double>> referenceGraph = read(h5file.openDataSet("refStar"));
    std::vector<std::pair<double, double>> targetGraph = read(h5file.openDataSet("targetStar"));

    std::vector<std::pair<size_t, size_t>> matches =
            astro::processing::graphmatching(referenceGraph, targetGraph, fullGraph, maxRatio);

    std::vector<std::pair<double, double>> refStars;
    std::vector<std::pair<double, double>> targetStars;
    for (auto pair : matches)
    {
        refStars.push_back(referenceGraph[pair.first]);
        targetStars.push_back(targetGraph[pair.second]);
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
