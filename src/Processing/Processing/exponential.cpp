#include "exponential.h"
#include "ui_exponential.h"

#include <IO/output.h>
#include <Plugin/pluginfactory.h>

#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>

namespace astro
{

Exponential::~Exponential() = default;

QString Exponential::name() const
{
    return "Exponential";
}

QString Exponential::explanation() const
{
    return tr("Raises the image values to the power of a parameter");
}

Exponential::GUI* Exponential::generateGUI(QWidget* parent) const
{
    return new ExponentialGUI(parent);
}

ExponentialGUI::ExponentialGUI(QWidget* parent)
    : MonoInterface::GUI(parent)
    , m_ui(std::make_unique<Ui::Exponential>())
{
    m_ui->setupUi(this);

    connect(m_ui->saveOutput, &QCheckBox::stateChanged, this, &ExponentialGUI::outputStateChanged);
    connect(m_ui->filenameOpen, &QPushButton::clicked, this, &ExponentialGUI::outputFileBoxOpen);
    connect(this, &ExponentialGUI::save, this, &ExponentialGUI::saveImg);
    connect(m_ui->skew, &QDoubleSpinBox::valueChanged, this, &ExponentialGUI::setSkewValue);
    connect(m_ui->skewSlider, &QSlider::sliderMoved, this, &ExponentialGUI::setApproximateSkewValue);
}

ExponentialGUI::~ExponentialGUI() = default;

void ExponentialGUI::outputStateChanged(int state)
{
    m_ui->filename->setEnabled(state != Qt::Unchecked);
    m_ui->filenameOpen->setEnabled(state != Qt::Unchecked);
}

void ExponentialGUI::outputFileBoxOpen()
{
    m_ui->filename->setText(QFileDialog::getSaveFileName(this, tr("Save output"), m_ui->filename->text()));
}

void ExponentialGUI::setSkewValue(double val)
{
    m_ui->skewSlider->setValue(std::roundl(-25 * std::log10(val)));
}

void ExponentialGUI::setApproximateSkewValue(int val)
{
    m_ui->skew->setValue(std::pow(10, -val / 25.));
}

bool ExponentialGUI::check()
{
    QFileInfo info(m_ui->filename->text());
    if (!info.exists())
    {
        return true;
    }
    int result = QMessageBox::question(this, tr("Exponential"), tr("Overwrite existing file?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
    if (result == QMessageBox::Abort)
    {
        return false;
    }
    m_overwriteIfExists = result == QMessageBox::Yes;

    return true;
}

ImageTypePtr ExponentialGUI::process(ImageTypePtr img, QPromise<void>& promise)
{
    float exponent = m_ui->skew->value();
    unsigned int nbDims = std::min(3U, img->GetNumberOfComponentsPerPixel());

    using DuplicatorType = itk::ImageDuplicator<ImageType>;
    auto duplicator = DuplicatorType::New();
    duplicator->SetInputImage(img);
    duplicator->Update();

    img = duplicator->GetOutput();
    using IteratorType = itk::ImageRegionIterator<ImageType>;

    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        for (unsigned int i = 0; i < nbDims; ++i)
        {
            value.SetElement(i, std::pow(value.GetElement(i), exponent));
        }
        it.Set(value);
        ++it;
    }

    emit save(img);

    return img;
}

void ExponentialGUI::saveImg(ImageTypePtr img)
{
    if (m_ui->saveOutput->checkState() == Qt::Checked)
    {
        OutputInterface::saveImg(img, m_ui->filename->text(), this);
    }
}

} // namespace astro
