#include "histostretch.h"
#include "ui_histostretch.h"
#include "ui_mono.h"

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>

namespace astro
{
HistoStretch::~HistoStretch() = default;

QString HistoStretch::name() const
{
    return "Exponential";
}

QString HistoStretch::explanation() const
{
    return tr("Raises the image values to the power of a parameter");
}

MonoInterfaceGUI* HistoStretch::generateGUI(QWidget* parent) const
{
    return new HistoStretchGUI(parent);
}

HistoStretchGUI::HistoStretchGUI(QWidget* parent)
    : MonoInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::HistoStretch>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_monoUi->setupUi(this, child);
    setTitle(tr("Histogram Stretch"));

    setupSlots();
    connect(m_ui->red, &QDoubleSpinBox::valueChanged, this, &HistoStretchGUI::setRedValue);
    connect(m_ui->redSlider, &QSlider::sliderMoved, this, &HistoStretchGUI::setApproximateRedValue);
    connect(m_ui->blue, &QDoubleSpinBox::valueChanged, this, &HistoStretchGUI::setBlueValue);
    connect(m_ui->blueSlider, &QSlider::sliderMoved, this, &HistoStretchGUI::setApproximateBlueValue);
    connect(m_ui->green, &QDoubleSpinBox::valueChanged, this, &HistoStretchGUI::setGreenValue);
    connect(m_ui->greenSlider, &QSlider::sliderMoved, this, &HistoStretchGUI::setApproximateGreenValue);
}

HistoStretchGUI::~HistoStretchGUI() = default;

void HistoStretchGUI::setRedValue(double val)
{
    m_ui->redSlider->setValue(val * 100);
}

void HistoStretchGUI::setApproximateRedValue(int val)
{
    m_ui->red->setValue(val / 100.);
}

void HistoStretchGUI::setBlueValue(double val)
{
    m_ui->blueSlider->setValue(val * 100);
}

void HistoStretchGUI::setApproximateBlueValue(int val)
{
    m_ui->blue->setValue(val / 100.);
}

void HistoStretchGUI::setGreenValue(double val)
{
    m_ui->greenSlider->setValue(val * 100);
}

void HistoStretchGUI::setApproximateGreenValue(int val)
{
    m_ui->green->setValue(val / 100.);
}

ImageTypePtr HistoStretchGUI::process(ImageTypePtr img, QPromise<void>& promise)
{
    std::array<float, 4> shift{{static_cast<float>(m_ui->red->value()), static_cast<float>(m_ui->blue->value()),
                                static_cast<float>(m_ui->green->value()), 1}};

    using DuplicatorType = itk::ImageDuplicator<ImageType>;
    auto duplicator = DuplicatorType::New();
    duplicator->SetInputImage(img);
    duplicator->Update();

    img = duplicator->GetOutput();
    using IteratorType = itk::ImageRegionIterator<ImageType>;

    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();

    unsigned int nbDims = img->GetNumberOfComponentsPerPixel();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        for (unsigned int i = 0; i < nbDims; ++i)
        {
            value.SetElement(i, std::max((value.GetElement(i) - shift[i]) / (1 - shift[i]), 0.f));
        }
        it.Set(value);
        ++it;
    }

    emit save(img);

    return img;
}
} // namespace astro
