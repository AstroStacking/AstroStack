#include "exponential.h"
#include "ui_exponential.h"

#include <Plugin/pluginfactory.h>

#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFileDialog>

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
    connect(this, &ExponentialGUI::setEnableState, this, &ExponentialGUI::setEnabled);
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
    return true;
}

ImageTypePtr ExponentialGUI::process(ImageTypePtr img, QPromise<void>& promise)
{
    emit setEnableState(false);

    if (m_ui->saveOutput->checkState() == Qt::Checked) {}

    emit setEnableState(true);

    return img;
}

} // namespace astro
