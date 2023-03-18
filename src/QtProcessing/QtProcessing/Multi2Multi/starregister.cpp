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
    connect(m_ui->threshold, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setThresholdValue);
    connect(m_ui->thresholdSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateThresholdValue);
    connect(m_ui->discardBigger, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setDiscardBiggerValue);
    connect(m_ui->discardBiggerSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateDiscardBiggerValue);
    connect(m_ui->fullGraph, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setFullGraphValue);
    connect(m_ui->fullGraphSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateFullGraphValue);
    connect(m_ui->maxRatio, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setMaxRatioValue);
    connect(m_ui->maxRatioSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateMaxRatioValue);
}

void StarRegisterGUI::setThresholdValue(double val)
{
    m_ui->thresholdSlider->setValue(static_cast<int>(val * 100));
}

void StarRegisterGUI::setApproximateThresholdValue(int val)
{
    m_ui->threshold->setValue(val / 100.);
}

void StarRegisterGUI::setDiscardBiggerValue(double val)
{
    m_ui->discardBiggerSlider->setValue(static_cast<int>(val));
}

void StarRegisterGUI::setApproximateDiscardBiggerValue(int val)
{
    m_ui->discardBigger->setValue(val);
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

    float threshold = m_ui->threshold->value();
    int discardBigger = static_cast<int>(m_ui->discardBigger->value());
    double maxRatio = m_ui->maxRatio->value();
    int fullGraph = static_cast<int>(m_ui->fullGraph->value());
    bool highDef = m_ui->highdef->isChecked();

    startNewTask(dims[0] * 5 - 1);

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
        processing::starDetection(greyDataset, index, starGroup, std::to_string(index), threshold, discardBigger);
        updateTask(index + dims[0]);
    }

    size_t middle = dims[0] / 2;
    H5::Group graphGroup = hdf5::getOrCreateGroup(m_graphGroupName, intermediateGroup);
    // Create graphs between current image and next one, save in inter/graphs
    for (size_t index = 0; index < middle; ++index)
    {
        std::vector<std::pair<double, double>> currentGraph =
                hdf5::readGraph<double>(starGroup.openDataSet(std::to_string(index)));
        std::vector<std::pair<double, double>> nextGraph =
                hdf5::readGraph<double>(starGroup.openDataSet(std::to_string(index + 1)));

        std::vector<std::pair<size_t, size_t>> matches =
                astro::processing::graphmatching(nextGraph, currentGraph, fullGraph, maxRatio);

        hdf5::writeGraph(matches, graphGroup, "partial" + std::to_string(index));

        updateTask(index + 2 * dims[0]);
    }
    for (size_t index = middle + 1; index < dims[0]; ++index)
    {
        std::vector<std::pair<double, double>> currentGraph =
                hdf5::readGraph<double>(starGroup.openDataSet(std::to_string(index)));
        std::vector<std::pair<double, double>> previousGraph =
                hdf5::readGraph<double>(starGroup.openDataSet(std::to_string(index - 1)));

        std::vector<std::pair<size_t, size_t>> matches =
                astro::processing::graphmatching(previousGraph, currentGraph, fullGraph, maxRatio);

        hdf5::writeGraph<size_t>(matches, graphGroup, "partial" + std::to_string(index));

        updateTask(index + 2 * dims[0] + middle);
    }

    H5::DataSet outputDataset = hdf5::createDataset(m_outputsDatasetName, dataspace, H5::PredType::NATIVE_FLOAT, group);
    std::vector<std::pair<double, double>> middleStars =
            hdf5::readGraph<double>(starGroup.openDataSet(std::to_string(middle)));
    std::vector<std::pair<size_t, size_t>> middleGraph;
    for (size_t i = 0; i < middleStars.size(); ++i)
    {
        middleGraph.push_back(std::make_pair(i, i));
    }
    std::vector<std::pair<size_t, size_t>> graph = middleGraph;

    // Create graphs
    for (size_t index = middle - 1; index < middle; --index)
    {
        std::vector<std::pair<size_t, size_t>> currentGraph =
                hdf5::readGraph<size_t>(graphGroup.openDataSet("partial" + std::to_string(index)));
        std::vector<std::pair<double, double>> currentStars =
                hdf5::readGraph<double>(starGroup.openDataSet(std::to_string(index)));

        // propagate graph
        std::vector<std::pair<size_t, size_t>> tempGraph;
        for (size_t i = 0; i < graph.size(); ++i)
        {
            auto it = std::find_if(currentGraph.begin(), currentGraph.end(),
                                   [=](std::pair<size_t, size_t> edge) { return graph[i].second == edge.first; });
            if (it != currentGraph.end())
            {
                tempGraph.emplace_back(graph[i].first, it->second);
            }
        }
        graph = std::move(tempGraph);

        std::vector<std::pair<double, double>> matchedMiddleStars;
        std::vector<std::pair<double, double>> matchedStars;

        // create graph
        for (size_t i = 0; i < graph.size(); ++i)
        {
            matchedMiddleStars.push_back(middleStars[graph[i].first]);
            matchedStars.push_back(currentStars[graph[i].second]);
        }

        hdf5::writeGraph<double>(matchedMiddleStars, graphGroup, "middle" + std::to_string(index));
        hdf5::writeGraph<double>(matchedStars, graphGroup, "current" + std::to_string(index));

        updateTask(index + 3 * dims[0]);
    }
    graph = middleGraph;
    for (size_t index = middle + 1; index < dims[0]; ++index)
    {
        std::vector<std::pair<size_t, size_t>> currentGraph =
                hdf5::readGraph<size_t>(graphGroup.openDataSet("partial" + std::to_string(index)));
        std::vector<std::pair<double, double>> currentStars =
                hdf5::readGraph<double>(starGroup.openDataSet(std::to_string(index)));

        // propagate graph
        std::vector<std::pair<size_t, size_t>> tempGraph;
        for (size_t i = 0; i < graph.size(); ++i)
        {
            auto it = std::find_if(currentGraph.begin(), currentGraph.end(),
                                   [=](std::pair<size_t, size_t> edge) { return graph[i].second == edge.first; });
            if (it != currentGraph.end())
            {
                tempGraph.emplace_back(graph[i].first, it->second);
            }
        }
        graph = std::move(tempGraph);

        std::vector<std::pair<double, double>> matchedMiddleStars;
        std::vector<std::pair<double, double>> matchedStars;

        // create graph
        for (size_t i = 0; i < graph.size(); ++i)
        {
            matchedMiddleStars.push_back(middleStars[graph[i].first]);
            matchedStars.push_back(currentStars[graph[i].second]);
        }

        hdf5::writeGraph<double>(matchedMiddleStars, graphGroup, "middle" + std::to_string(index));
        hdf5::writeGraph<double>(matchedStars, graphGroup, "current" + std::to_string(index));

        updateTask(index + 3 * dims[0] + middle);
    }

    // Create transformation, apply on image
    ImageTypePtr middleImg = hdf5::extractFrom(inputsDataset, middle);
    for (size_t index = 0; index < middle; ++index)
    {
        ImageTypePtr img = hdf5::extractFrom(inputsDataset, index);
        std::vector<std::pair<double, double>> middleStars =
                hdf5::readGraph<double>(graphGroup.openDataSet("middle" + std::to_string(index)));
        std::vector<std::pair<double, double>> currentStars =
                hdf5::readGraph<double>(graphGroup.openDataSet("current" + std::to_string(index)));
        ImageTypePtr registeredImg =
                highDef ? processing::registerImagesBSpline(middleImg, img, middleStars, currentStars)
                        : processing::registerImages(middleImg, img, middleStars, currentStars);

        hdf5::writeTo(*registeredImg, outputDataset, index);
        updateTask(index + 4 * dims[0]);
    }
    {
        ImageTypePtr middleImg = hdf5::extractFrom(inputsDataset, middle);
        hdf5::writeTo(*middleImg, outputDataset, middle);
        updateTask(4 * dims[0] + middle);
    }
    // Copy middle image
    for (size_t index = middle + 1; index < dims[0]; ++index)
    {
        ImageTypePtr img = hdf5::extractFrom(inputsDataset, index);
        std::vector<std::pair<double, double>> middleStars =
                hdf5::readGraph<double>(graphGroup.openDataSet("middle" + std::to_string(index)));
        std::vector<std::pair<double, double>> currentStars =
                hdf5::readGraph<double>(graphGroup.openDataSet("current" + std::to_string(index)));
        ImageTypePtr registeredImg =
                highDef ? processing::registerImagesBSpline(middleImg, img, middleStars, currentStars)
                        : processing::registerImages(middleImg, img, middleStars, currentStars);

        hdf5::writeTo(*registeredImg, outputDataset, index);
        updateTask(index + 4 * dims[0] + middle + 1);
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
    m_ui->threshold->setValue(settings.value("threshold").toInt());
    m_ui->discardBigger->setValue(settings.value("discardBigger").toInt());
    m_ui->fullGraph->setValue(settings.value("fullGraph").toInt());
    m_ui->maxRatio->setValue(settings.value("maxRatio").toDouble());
}

void StarRegisterGUI::save(QSettings& settings)
{
    settings.setValue("threshold", m_ui->threshold->value());
    settings.setValue("discardBigger", m_ui->discardBigger->value());
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
