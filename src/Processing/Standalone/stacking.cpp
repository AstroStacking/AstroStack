#include <IO/io.h>
#include <IO/itkinput.h>
#include <IO/itkoutput.h>
#include <Processing/maxstacking.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <QtCore/QTemporaryFile>

#include <itkImportImageFilter.h>

#include <H5Cpp.h>

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

    hsize_t maxDims[4]{static_cast<hsize_t>(filenames.size()), size.at(0), size.at(1), size.at(2)};
    hsize_t inputDim[4]{1, size.at(0), size.at(1), size.at(2)};
    H5::DataSpace inputSpace(4, inputDim);
    H5::DataSet inputsDataset = h5file.createDataSet(inputsDatasetName, H5::PredType::NATIVE_FLOAT, inputSpace);

    for (size_t i = 0; i < filenames.size(); ++i)
    {
        H5::DataSpace fspace1 = inputsDataset.getSpace();
        hsize_t offset[4] = {i, 0, 0, 0};
        hsize_t currSlab[4]{static_cast<hsize_t>(i), size.at(0), size.at(1), size.at(2)};
        fspace1.selectHyperslab(H5S_SELECT_SET, currSlab, offset);

        auto img = astro::io::open(filenames[i].toStdString());

        inputsDataset.write(img.GetPointer(), H5::PredType::NATIVE_FLOAT, inputSpace, fspace1);
    }

    hsize_t outputImgDim[3]{size.at(0), size.at(1), size.at(2)};
    H5::DataSpace outputSpace(3, outputImgDim);
    H5::DataSet outputDataset = h5file.createDataSet(outputDatasetName, H5::PredType::NATIVE_FLOAT, outputSpace);

    astro::processing::maxStacking(inputsDataset, outputDataset);

    {
        using ImportFilterType = itk::ImportImageFilter<astro::PixelType, astro::Dimension>;
        auto importFilter = ImportFilterType::New();
        ImportFilterType::SizeType regionSize;

        regionSize[0] = size.at(0); // size along X
        regionSize[1] = size.at(1); // size along Y

        ImportFilterType::IndexType start;
        start.Fill(0);

        ImportFilterType::RegionType region;
        region.SetIndex(start);
        region.SetSize(regionSize);
        importFilter->SetRegion(region);

        const itk::SpacePrecisionType origin[astro::Dimension] = {0.0, 0.0};
        importFilter->SetOrigin(origin);
        const itk::SpacePrecisionType spacing[astro::Dimension] = {1.0, 1.0};
        importFilter->SetSpacing(spacing);

        const bool importImageFilterWillOwnTheBuffer = false;
        std::vector<astro::PixelType> buffer(size.at(0) * size.at(1));
        outputDataset.read(buffer.data(), H5::PredType::NATIVE_FLOAT, outputSpace, outputSpace);

        importFilter->SetImportPointer(buffer.data(), buffer.size(), importImageFilterWillOwnTheBuffer);
        importFilter->Update();
        refImg.setImg(importFilter->GetOutput());
    }

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
