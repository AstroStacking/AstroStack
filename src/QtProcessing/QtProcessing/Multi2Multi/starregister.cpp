#include "starregister.h"
#include "../ui_multi2multiinterface.h"
#include "ui_starregister.h"
#include <Processing/graphmatching.h>
#include <Processing/grey.h>
#include <Processing/register.h>
#include <Processing/stardetection.h>

#include <QtCore/QJsonObject>

namespace astro
{

StarRegister::~StarRegister() = default;

QString StarRegister::name() const
{
    return "StarRegister";
}

QString StarRegister::explanation() const
{
    return tr("Registers images on a middle one");
}

Multi2MultiInterfaceGUI* StarRegister::generateGUI(QWidget* parent) const
{
    return new StarRegisterGUI(parent);
}

StarRegisterGUI::StarRegisterGUI(QWidget* parent)
    : Multi2MultiInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::StarRegister>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_multiUi->setupUi(this, child);
    setTitle(tr("StarRegister"));

    setupSlots();
}

StarRegisterGUI::~StarRegisterGUI() = default;

void StarRegisterGUI::setupSlots()
{
    connect(m_ui->minStars, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setMinStarsValue);
    connect(m_ui->minStarsSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateMinStarsValue);
    connect(m_ui->maxStars, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setMaxStarsValue);
    connect(m_ui->maxStarsSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateMaxStarsValue);
    connect(m_ui->fullGraph, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setFullGraphValue);
    connect(m_ui->fullGraphSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateFullGraphValue);
    connect(m_ui->maxRatio, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setMaxRatioValue);
    connect(m_ui->maxRatioSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateMaxRatioValue);
}

void StarRegisterGUI::setMinStarsValue(double val)
{
    m_ui->minStarsSlider->setValue(static_cast<int>(val));
}

void StarRegisterGUI::setApproximateMinStarsValue(int val)
{
    m_ui->minStars->setValue(val);
}

void StarRegisterGUI::setMaxStarsValue(double val)
{
    m_ui->maxStarsSlider->setValue(static_cast<int>(val));
}

void StarRegisterGUI::setApproximateMaxStarsValue(int val)
{
    m_ui->maxStars->setValue(val);
}

void StarRegisterGUI::setFullGraphValue(double val)
{
    m_ui->fullGraphSlider->setValue(static_cast<int>(val));
}

void StarRegisterGUI::setApproximateFullGraphValue(int val)
{
    m_ui->fullGraph->setValue(val);
}

void StarRegisterGUI::setMaxRatioValue(double val)
{
    m_ui->maxRatioSlider->setValue(static_cast<int>(val * 100));
}

void StarRegisterGUI::setApproximateMaxRatioValue(int val)
{
    m_ui->maxRatio->setValue(val / 100.);
}

void StarRegisterGUI::process(const H5::H5File& group, const StartTask& startNewTask, const UpdateTask& updateTask,
                              QPromise<void>& promise)
try
{
    H5::DataSet inputsDataset = group.openDataSet(m_inputsDatasetName);
    H5::DataSpace dataspace = inputsDataset.getSpace();
    int ndims = dataspace.getSimpleExtentNdims();
    if (ndims != 4)
    {
        throw std::runtime_error("Wrong number of dimensions");
    }
    hsize_t dims[4];
    ndims = dataspace.getSimpleExtentDims(dims, nullptr);

    int minStars = static_cast<int>(m_ui->minStars->value());
    int maxStars = static_cast<int>(m_ui->maxStars->value());
    double maxRatio = m_ui->maxRatio->value();
    int fullGraph = static_cast<int>(m_ui->fullGraph->value());

    startNewTask(dims[0] * 4 - 1);

    H5::Group intermediateGroup = hdf5::getOrCreateGroup(m_intermediateGroupName, group);

    // Grey all images, save in inter/greys
    hsize_t greyDims[3]{dims[0], dims[1], dims[2]};
    H5::DataSpace greyDataspace(3, greyDims);
    H5::DataSet greyDataset =
            hdf5::createDataset(m_greyDatasetName, greyDataspace, H5::PredType::NATIVE_FLOAT, intermediateGroup);
    // create grey dataset
    for (size_t index = 0; index < dims[0]; ++index)
    {
        ImageTypePtr img = hdf5::extractFrom(inputsDataset, index);
        auto outputImg = processing::grey(img);
        hdf5::writeTo(*outputImg, greyDataset, index);
        updateTask(index);
    }

    H5::Group starGroup = hdf5::getOrCreateGroup(m_starsGroupName, intermediateGroup);
    // Star detection, save in inter/stars
    for (size_t index = 0; index < dims[0]; ++index)
    {
        processing::starDetection(greyDataset, index, starGroup, std::to_string(index), minStars, maxStars);
        updateTask(index + dims[0]);
    }

    size_t middle = dims[0] / 2;
    std::vector<std::pair<double, double>> middleGraph = hdf5::readGraph(starGroup.openDataSet(std::to_string(middle)));
    H5::Group graphGroup = hdf5::getOrCreateGroup(m_graphGroupName, intermediateGroup);
    // Create graphs, save in inter/graphs
    for (size_t index = 0; index < middle; ++index)
    {
        std::vector<std::pair<double, double>> currentGraph =
                hdf5::readGraph(starGroup.openDataSet(std::to_string(index)));

        std::vector<std::pair<size_t, size_t>> matches =
                astro::processing::graphmatching(middleGraph, currentGraph, fullGraph, maxRatio);

        std::vector<std::pair<double, double>> middleStars;
        std::vector<std::pair<double, double>> currentStars;
        for (auto pair : matches)
        {
            middleStars.push_back(middleGraph[pair.first]);
            currentStars.push_back(currentGraph[pair.second]);
        }
        hdf5::writeGraph(middleStars, graphGroup, "middle" + std::to_string(index));
        hdf5::writeGraph(currentStars, graphGroup, "current" + std::to_string(index));

        updateTask(index + 2 * dims[0]);
    }
    for (size_t index = middle + 1; index < dims[0]; ++index)
    {
        std::vector<std::pair<double, double>> currentGraph =
                hdf5::readGraph(starGroup.openDataSet(std::to_string(index)));

        std::vector<std::pair<size_t, size_t>> matches =
                astro::processing::graphmatching(middleGraph, currentGraph, fullGraph, maxRatio);

        std::vector<std::pair<double, double>> middleStars;
        std::vector<std::pair<double, double>> currentStars;
        for (auto pair : matches)
        {
            middleStars.push_back(middleGraph[pair.first]);
            currentStars.push_back(currentGraph[pair.second]);
        }
        hdf5::writeGraph(middleStars, graphGroup, "middle" + std::to_string(index));
        hdf5::writeGraph(currentStars, graphGroup, "current" + std::to_string(index));

        updateTask(index + 2 * dims[0] + middle);
    }

    H5::DataSet outputDataset = hdf5::createDataset(m_outputsDatasetName, dataspace, H5::PredType::NATIVE_FLOAT, group);

    // Create transformation, apply on image
    ImageTypePtr middleImg = hdf5::extractFrom(inputsDataset, middle);
    for (size_t index = 0; index < middle; ++index)
    {
        ImageTypePtr img = hdf5::extractFrom(inputsDataset, index);
        std::vector<std::pair<double, double>> middleStars =
                hdf5::readGraph(graphGroup.openDataSet("middle" + std::to_string(index)));
        std::vector<std::pair<double, double>> currentStars =
                hdf5::readGraph(graphGroup.openDataSet("current" + std::to_string(index)));
        ImageTypePtr registeredImg = processing::registerImages(middleImg, img, middleStars, currentStars);

        hdf5::writeTo(*img, outputDataset, index);
        updateTask(index + 3 * dims[0]);
    }
    {
        ImageTypePtr middleImg = hdf5::extractFrom(inputsDataset, middle);
        hdf5::writeTo(*middleImg, outputDataset, middle);
        updateTask(3 * dims[0] + middle);
    }
    // Copy middle image
    for (size_t index = middle + 1; index < dims[0]; ++index)
    {
        ImageTypePtr img = hdf5::extractFrom(inputsDataset, index);
        std::vector<std::pair<double, double>> middleStars =
                hdf5::readGraph(graphGroup.openDataSet("middle" + std::to_string(index)));
        std::vector<std::pair<double, double>> currentStars =
                hdf5::readGraph(graphGroup.openDataSet("current" + std::to_string(index)));
        ImageTypePtr registeredImg = processing::registerImages(middleImg, img, middleStars, currentStars);

        hdf5::writeTo(*img, outputDataset, index);
        updateTask(index + 3 * dims[0] + middle + 1);
    }
}
catch (const std::exception& e)
{
    promise.setException(std::current_exception());
}

void StarRegisterGUI::restore(QSettings& settings)
{
    if (!settings.contains("minStars"))
    {
        return;
    }
    m_ui->minStars->setValue(settings.value("minStars").toInt());
    m_ui->maxStars->setValue(settings.value("maxStars").toInt());
    m_ui->fullGraph->setValue(settings.value("fullGraph").toInt());
    m_ui->maxRatio->setValue(settings.value("maxRatio").toDouble());
}

void StarRegisterGUI::save(QSettings& settings)
{
    settings.setValue("minStars", m_ui->minStars->value());
    settings.setValue("maxStars", m_ui->maxStars->value());
    settings.setValue("fullGraph", m_ui->fullGraph->value());
    settings.setValue("maxRatio", m_ui->maxRatio->value());
}

bool StarRegisterGUI::check()
{
    return true;
}

void StarRegisterGUI::setup(QJsonObject data)
{
    Multi2MultiInterfaceGUI::setup(data);
    auto inputs = data["Inputs"].toObject();
    m_inputsDatasetName = inputs["inputs"].toObject()["dataset"].toString().toStdString();
    auto outputs = data["Outputs"].toObject();
    m_outputsDatasetName = outputs["outputs"].toObject()["dataset"].toString().toStdString();
    auto intermediate = outputs["intermediate"].toObject();
    m_intermediateGroupName = intermediate["group"].toString().toStdString();
    m_greyDatasetName = intermediate["greys"].toObject()["dataset"].toString().toStdString();
    m_starsGroupName = intermediate["stars"].toObject()["group"].toString().toStdString();
    m_graphGroupName = intermediate["graphs"].toObject()["group"].toString().toStdString();
}

} // namespace astro
