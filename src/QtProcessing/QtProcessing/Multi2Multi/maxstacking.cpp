#include "maxstacking.h"
#include "../ui_multi2multiinterface.h"
#include "ui_maxstacking.h"
#include <Algos/Filters/Stackers/max.h>
#include <IO/hdf5.h>
#include <Processing/chromasmoothing.h>
#include <Processing/stacking.h>
#include <QtIO/output.h>

#include <QtCore/QJsonObject>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

namespace astro
{

MaxStacking::~MaxStacking() = default;

QString MaxStacking::name() const
{
    return "MaxStacking";
}

QString MaxStacking::explanation() const
{
    return tr("Computes the max of a pixel among images");
}

Multi2MultiInterfaceGUI* MaxStacking::generateGUI(QWidget* parent) const
{
    return new MaxStackingGUI(parent);
}

MaxStackingGUI::MaxStackingGUI(QWidget* parent)
    : Multi2MultiInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::MaxStacking>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_multiUi->setupUi(this, child);
    setTitle(tr("MaxStacking"));

    setupSlots();
}

MaxStackingGUI::~MaxStackingGUI() = default;

void MaxStackingGUI::setupSlots()
{
    connect(m_ui->filenameOpen, &QPushButton::clicked, this, &MaxStackingGUI::outputFileBoxOpen);
    connect(this, &MaxStackingGUI::save, this, &MaxStackingGUI::saveImg);
}

void MaxStackingGUI::setup(QJsonObject data)
{
    auto inputs = data["Inputs"].toObject();
    m_inputsDatasetName = inputs["data"].toObject()["dataset"].toString().toStdString();
    auto outputs = data["Outputs"].toObject();
    m_outputDatasetName = outputs["inputs"].toObject()["dataset"].toString().toStdString();
}

void MaxStackingGUI::outputFileBoxOpen()
{
    m_ui->filename->setText(QFileDialog::getSaveFileName(this, tr("Save output"), m_ui->filename->text()));
}

bool MaxStackingGUI::check()
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

void MaxStackingGUI::setNextFilename(QFileInfo info, QString basename, long inc)
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

void MaxStackingGUI::saveImg(const AstroImage& img)
{
    QFileInfo info(m_ui->filename->text());
    if (!info.exists() || m_overwriteIfExists)
    {
        OutputInterface::saveImg(img, m_ui->filename->text(), this);
    }
}

void MaxStackingGUI::process(H5::Group group, QPromise<void>& promise)
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

    processing::stacking(inputsDataset, outputDataset, filters::stackers::Max<float>());

    AstroImage img;
    img.setImg(hdf5::extractFrom(outputDataset));

    emit save(img);
}
} // namespace astro
