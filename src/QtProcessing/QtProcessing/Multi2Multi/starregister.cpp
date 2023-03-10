#include "starregister.h"
#include "../ui_multi2multiinterface.h"
#include "ui_starregister.h"

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

void StarRegisterGUI::process(const H5::H5File& group, const std::function<void(int)>& startNewTask,
                              const std::function<void(int)>& updateTask, QPromise<void>& promise)
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

    startNewTask(dims[0] * 4 - 1);

    H5::Group intermediateGroup = hdf5::getOrCreateGroup(m_intermediateGroupName.toStdString(), group);

    // Grey all images, save in inter/greys
    // Star detection, save in inter/stars
    // Create graphs, save in inter/graphs
    // Create transformation, apply on image

    H5::DataSet outputDataset =
            hdf5::createDataset(m_outputsDatasetName.toStdString(), dataspace, H5::PredType::NATIVE_FLOAT, group);

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
