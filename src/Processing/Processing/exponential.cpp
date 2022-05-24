#include "exponential.h"
#include "ui_exponential.h"

#include <Plugin/pluginfactory.h>

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


ImageTypePtr ExponentialGUI::process(ImageTypePtr img, QPromise<void>& promise)
{
    return img;
}

} // namespace astro
