#include "robustmeanstacking.h"
#include "../ui_multi2multiinterface.h"
#include "ui_robustmeanstacking.h"
#include <Algos/Filters/Stackers/robustmean.h>
#include <IO/hdf5.h>
#include <Processing/stacking.h>
#include <QtIO/output.h>

#include <QtCore/QJsonObject>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

namespace astro
{

RobustMeanStacking::~RobustMeanStacking() = default;

QString RobustMeanStacking::name() const
{
    return "RobustMeanStacking";
}

QString RobustMeanStacking::explanation() const
{
    return tr("Computes the robust mean of a pixel among images");
}

Multi2MultiInterfaceGUI* RobustMeanStacking::generateGUI(QWidget* parent) const
{
    return new RobustMeanStackingGUI(parent);
}

RobustMeanStackingGUI::RobustMeanStackingGUI(QWidget* parent)
    : Multi2MultiInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::RobustMeanStacking>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_multiUi->setupUi(this, child);
    setTitle(tr("RobustMeanStackingGUI"));

    setupSlots();
}

RobustMeanStackingGUI::~RobustMeanStackingGUI() = default;

void RobustMeanStackingGUI::setupSlots()
{
    connect(m_ui->filenameOpen, &QPushButton::clicked, this, &RobustMeanStackingGUI::outputFileBoxOpen);
    connect(m_ui->variance, &QDoubleSpinBox::valueChanged, this, &RobustMeanStackingGUI::setVarianceValue);
    connect(m_ui->varianceSlider, &QSlider::valueChanged, this, &RobustMeanStackingGUI::setApproximateVarianceValue);
}

void RobustMeanStackingGUI::setVarianceValue(double val)
{
    m_ui->varianceSlider->setValue(static_cast<int>(val));
}

void RobustMeanStackingGUI::setApproximateVarianceValue(int val)
{
    m_ui->variance->setValue(val);
}

void RobustMeanStackingGUI::setup(QJsonObject data)
{
    Multi2MultiInterfaceGUI::setup(data);
    auto inputs = data["Inputs"].toObject();
    m_inputsDatasetName = inputs["inputs"].toObject()["dataset"].toString().toStdString();
    auto outputs = data["Outputs"].toObject();
    m_outputDatasetName = outputs["output"].toObject()["dataset"].toString().toStdString();
}

void RobustMeanStackingGUI::outputFileBoxOpen()
{
    m_ui->filename->setText(QFileDialog::getSaveFileName(this, tr("Save output"), m_ui->filename->text()));
}

void RobustMeanStackingGUI::restore(QSettings& settings)
{
    if (!settings.contains("file"))
    {
        return;
    }
    m_ui->filename->setText(settings.value("file").toString());
}

void RobustMeanStackingGUI::save(QSettings& settings)
{
    settings.setValue("file", m_ui->filename->text());
}

bool RobustMeanStackingGUI::check()
{
    QFileInfo info(m_ui->filename->text());
    if (!info.exists())
    {
        return true;
    }
    int result = QMessageBox::question(this, m_name, tr("Overwrite existing file?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
    if (result == QMessageBox::Abort)
    {
        return false;
    }
    m_overwriteIfExists = result == QMessageBox::Yes;
    if (!m_overwriteIfExists)
    {
        QFileInfo info(m_ui->filename->text());
        QString basename = info.completeBaseName();
        QRegularExpression regex("(.*)_(\\d+)$");
        auto match = regex.match(basename);
        if (match.hasMatch())
        {
            setNextFilename(info, match.captured(1), match.captured(2).toLong());
        }
        else
        {
            setNextFilename(info, basename, 1);
        }
    }

    return true;
}

void RobustMeanStackingGUI::setNextFilename(QFileInfo info, QString basename, long inc)
{
    while (true)
    {
        QString potentialFilename =
                info.dir().filePath(basename + "_" + QString::number(inc) + "." + info.completeSuffix());
        if (!QFileInfo(potentialFilename).exists())
        {
            m_ui->filename->setText(potentialFilename);
            break;
        }
        ++inc;
    }
}

void RobustMeanStackingGUI::saveImg(const AstroImage& img)
{
    QFileInfo info(m_ui->filename->text());
    if (!info.exists() || m_overwriteIfExists)
    {
        OutputInterface::saveImg(img, m_ui->filename->text(), this);
    }
}

void RobustMeanStackingGUI::process(const H5::H5File& group, const StartTask& startNewTask,
                                    const UpdateTask& updateTask, QPromise<void>& promise)
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
    if (dims[3] != astro::PixelDimension)
    {
        throw std::runtime_error("Wrong pixel dimension");
    }

    hsize_t outputImgDim[3]{dims[1], dims[2], astro::PixelDimension};
    H5::DataSpace outputSpace(3, outputImgDim);
    H5::DataSet outputDataset =
            hdf5::createDataset(m_outputDatasetName, outputSpace, H5::PredType::NATIVE_FLOAT, group);
    startNewTask(dims[1]);

    processing::stacking(inputsDataset, outputDataset, filters::stackers::RobustMean<float>(m_ui->variance->value()),
                         updateTask);

    AstroImage img;
    img.setImg(hdf5::extractFrom(outputDataset));
    saveImg(img);
}
catch (const std::exception& e)
{
    promise.setException(std::current_exception());
}
} // namespace astro
