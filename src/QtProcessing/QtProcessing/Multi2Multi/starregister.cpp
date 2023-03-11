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

void StarRegisterGUI::process(const H5::H5File& group, const StartTask& startNewTask,
                              const UpdateTask& updateTask, QPromise<void>& promise)
try
{
    H5::DataSet inputs = group.openDataSet(m_inputsDatasetName);
    H5::DataSpace dataspace = inputs.getSpace();
    int ndims = dataspace.getSimpleExtentNdims();
    if (ndims != 4)
    {
        throw std::runtime_error("Wrong number of dimensions");
    }
    hsize_t dims[4];
    ndims = dataspace.getSimpleExtentDims(dims, nullptr);

    int minStars = static_cast<int>(m_ui->minStars->value());
    int maxStars = static_cast<int>(m_ui->maxStars->value());

    startNewTask(dims[0] * 4 - 1);

    H5::Group intermediateGroup = hdf5::getOrCreateGroup(m_intermediateGroupName.toStdString(), group);

    // Grey all images, save in inter/greys
    hsize_t greyDims[3]{dims[0], dims[1], dims[2]};
    H5::DataSpace greyDataspace(3, greyDims);
    H5::DataSet greyDataset = hdf5::createDataset(m_greyDatasetName.toStdString(), greyDataspace,
                                                  H5::PredType::NATIVE_FLOAT, intermediateGroup);
    // create grey dataset
    for (size_t index = 0; index < dims[0]; ++index)
    {
        ImageTypePtr img = hdf5::extractFrom(inputs, index);
        auto outputImg = processing::grey(img);
        hdf5::writeTo(*outputImg, greyDataset, index);
        updateTask(index);
    }

    H5::Group starGroup = hdf5::getOrCreateGroup(m_starsGroupName.toStdString(), intermediateGroup);

    // Star detection, save in inter/stars
    for (size_t index = 0; index < dims[0]; ++index)
    {
        processing::starDetection(greyDataset, index, starGroup, std::to_string(index), minStars, maxStars);
        updateTask(index + dims[0]);
    }

    size_t middle = dims[0] / 2;
    
    // Create graphs, save in inter/graphs
    for (size_t index = 0; index < middle; ++index)
    {
        updateTask(index + 2 * dims[0]);
    }
    for (size_t index = middle + 1; index < dims[0]; ++index)
    {
        updateTask(index + 2 * dims[0] + middle);
    }

    H5::DataSet outputDataset = hdf5::createDataset(m_outputsDatasetName.toStdString(), dataspace,
                                                  H5::PredType::NATIVE_FLOAT, group);

    // Create transformation, apply on image
    for (size_t index = 0; index < middle; ++index)
    {
        updateTask(index + 3 * dims[0]);
    }
    {
        updateTask(3 * dims[0] + middle);
    }
    // Copy middle image
    for (size_t index = middle + 1; index < dims[0]; ++index)
    {
        updateTask(index + 3 * dims[0] + middle + 1);
    }

    // for()
    // writeTo

    /*
     astro::ScalarImageTypePtr fixGrey = astro::processing::grey(inputs, 0, h5file, "fixGrey");
     astro::ScalarImageTypePtr movingGrey = astro::processing::grey(inputs, 1, h5file, "movingGrey");

     astro::processing::starDetection(h5file.openDataSet("fixGrey"), h5file, "fixStar", minStars, maxStars);
     astro::processing::starDetection(h5file.openDataSet("movingGrey"), h5file, "movingStar", minStars, maxStars);

     std::vector<std::pair<double, double>> fixGraph = read(h5file.openDataSet("fixStar"));
     std::vector<std::pair<double, double>> movingGraph = read(h5file.openDataSet("movingStar"));

     std::vector<std::pair<size_t, size_t>> matches =
             astro::processing::graphmatching(fixGraph, movingGraph, fullGraph, maxRatio);

     std::vector<std::pair<double, double>> fixStars;
     std::vector<std::pair<double, double>> movingStars;
     for (auto pair : matches)
     {
         fixStars.push_back(fixGraph[pair.first]);
         movingStars.push_back(movingGraph[pair.second]);
     }

     astro::ImageTypePtr outputImg =
             astro::processing::registerImages(fixImg.getImg(), movingImg.getImg(), fixStars, movingStars);

     */
}
catch (const std::exception& e)
{
    promise.setException(std::current_exception());
}

void StarRegisterGUI::restore(QSettings& settings) {}

void StarRegisterGUI::save(QSettings& settings) {}

bool StarRegisterGUI::check()
{
    return true;
}

void StarRegisterGUI::setup(QJsonObject data)
{
    Multi2MultiInterfaceGUI::setup(data);
    auto outputs = data["Outputs"].toObject();
    m_outputsDatasetName = outputs["outputs"].toObject()["dataset"].toString();
    auto intermediate = data["Intermediate"].toObject();
    m_intermediateGroupName = intermediate["group"].toString();
    m_greyDatasetName = intermediate["greys"].toObject()["dataset"].toString();
    m_starsGroupName = intermediate["stars"].toObject()["group"].toString();
    m_graphGroupName = intermediate["graphs"].toObject()["group"].toString();
}

} // namespace astro
