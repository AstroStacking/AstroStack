#include "medianstacking.h"
#include "../ui_multi2multiinterface.h"
#include "ui_medianstacking.h"
#include <Algos/Filters/Stackers/median.h>
#include <IO/hdf5.h>
#include <Processing/stacking.h>
#include <QtIO/output.h>

#include <QtCore/QJsonObject>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

namespace astro
{

MedianStacking::~MedianStacking() = default;

QString MedianStacking::name() const
{
    return "MedianStacking";
}

QString MedianStacking::explanation() const
{
    return tr("Computes the median of a pixel among images");
}

Multi2MultiInterfaceGUI* MedianStacking::generateGUI(QWidget* parent) const
{
    return new MedianStackingGUI(parent);
}

MedianStackingGUI::MedianStackingGUI(QWidget* parent)
    : Multi2MultiInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::MedianStacking>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_multiUi->setupUi(this, child);
    setTitle(tr("MedianStacking"));

    setupSlots();
}

MedianStackingGUI::~MedianStackingGUI() = default;

void MedianStackingGUI::setupSlots()
{
    connect(m_ui->filenameOpen, &QPushButton::clicked, this, &MedianStackingGUI::outputFileBoxOpen);
}

void MedianStackingGUI::setup(QJsonObject data)
{
    Multi2MultiInterfaceGUI::setup(data);
    auto inputs = data["Inputs"].toObject();
    m_inputsDatasetName = inputs["inputs"].toObject()["dataset"].toString().toStdString();
    auto outputs = data["Outputs"].toObject();
    m_outputDatasetName = outputs["output"].toObject()["dataset"].toString().toStdString();
}

void MedianStackingGUI::outputFileBoxOpen()
{
    m_ui->filename->setText(QFileDialog::getSaveFileName(this, tr("Save output"), m_ui->filename->text()));
}

void MedianStackingGUI::restore(QSettings& settings)
{
    if (!settings.contains("file"))
    {
        return;
    }
    m_ui->filename->setText(settings.value("file").toString());
}

void MedianStackingGUI::save(QSettings& settings)
{
    settings.setValue("file", m_ui->filename->text());
}

bool MedianStackingGUI::check()
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

void MedianStackingGUI::setNextFilename(QFileInfo info, QString basename, long inc)
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

void MedianStackingGUI::saveImg(const AstroImage& img)
{
    QFileInfo info(m_ui->filename->text());
    if (!info.exists() || m_overwriteIfExists)
    {
        OutputInterface::saveImg(img, m_ui->filename->text(), this);
    }
}

void MedianStackingGUI::process(const H5::H5File& group, const StartTask& startNewTask, const UpdateTask& updateTask,
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
    if (dims[3] != astro::PixelDimension)
    {
        throw std::runtime_error("Wrong pixel dimension");
    }

    hsize_t outputImgDim[3]{dims[1], dims[2], astro::PixelDimension};
    H5::DataSpace outputSpace(3, outputImgDim);
    H5::DataSet outputDataset = hdf5::createDataset<float>(m_outputDatasetName, outputSpace, group);
    startNewTask(dims[1] + 1, tr("Median stacking"));

    processing::stacking(inputsDataset, outputDataset, filters::stackers::Median<float>(), updateTask);

    AstroImage img;
    img.setImg(hdf5::extractFrom(outputDataset));
    saveImg(img);
    updateTask();
}
catch (const std::exception& e)
{
    promise.setException(std::current_exception());
}
} // namespace astro
