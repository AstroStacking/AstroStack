#include "reader.h"
#include "../ui_multi2multiinterface.h"
#include "ui_reader.h"
#include <IO/hdf5.h>
#include <QtIO/input.h>

#include <QtCore/QJsonObject>
#include <QtGui/QFileSystemModel>
#include <QtWidgets/QDoubleSpinBox>

namespace astro
{

Reader::~Reader() = default;

QString Reader::name() const
{
    return "Reader";
}

QString Reader::explanation() const
{
    return tr("Reads images");
}

Multi2MultiInterfaceGUI* Reader::generateGUI(QWidget* parent) const
{
    return new ReaderGUI(parent);
}

ReaderGUI::ReaderGUI(QWidget* parent)
    : Multi2MultiInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::Reader>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_multiUi->setupUi(this, child);
    setTitle(tr("Reader"));

    m_model = std::make_unique<QFileSystemModel>();
    m_model->setRootPath(QDir::rootPath());
    QStringList filters;
    // TODO plugins to filters
    filters << "*.png";
    filters << "*.jpg";
    filters << "*.tif";
    filters << "*.tiff";

    m_model->setNameFilters(filters);
    m_model->setNameFilterDisables(false);
    m_ui->treeView->setModel(m_model.get());
    m_ui->treeView->setCurrentIndex(m_model->index(QDir::homePath()));
    m_ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

ReaderGUI::~ReaderGUI() = default;

void ReaderGUI::process(H5::Group group, QPromise<void>& promise)
{
    auto indices = m_ui->treeView->selectionModel()->selectedIndexes();

    QStringList filenames;

    for (auto index : indices)
    {
        filenames.push_back(m_model->fileInfo(index).absoluteFilePath());
    }
    if (filenames.empty())
    {
        throw std::runtime_error("No file selected");
    }

    auto refImg = InputInterface::loadImg(filenames.front(), this);
    auto size = refImg.getImg()->GetRequestedRegion().GetSize();

    std::vector<std::string> transformedFilenames;
    for (QString filename : filenames)
    {
        transformedFilenames.push_back(filename.toStdString());
    }

    astro::hdf5::readTo(transformedFilenames, size, group, m_outputDatasetName.toStdString());
}

void ReaderGUI::setup(QJsonObject data)
{
    auto outputs = data["Outputs"].toObject();
    m_outputDatasetName = outputs["data"].toObject()["dataset"].toString();
}

} // namespace astro
