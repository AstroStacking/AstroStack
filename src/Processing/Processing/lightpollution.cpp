#include "lightpollution.h"
#include "ui_lightpollution.h"
#include "ui_monointerface.h"

#include <QtWidgets/QDoubleSpinBox>

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>

namespace astro
{

LightPollution::~LightPollution() = default;

QString LightPollution::name() const
{
    return "LightPollution";
}

QString LightPollution::explanation() const
{
    return tr("Estiates and removes the light pollution");
}

MonoInterfaceGUI* LightPollution::generateGUI(QWidget* parent) const
{
    return new LightPollutionGUI(parent);
}

LightPollutionGUI::LightPollutionGUI(QWidget* parent)
    : MonoInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::LightPollution>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_monoUi->setupUi(this, child);
    setTitle(tr("LightPollution"));

    setupSlots();
}

LightPollutionGUI::~LightPollutionGUI() = default;

AstroImage LightPollutionGUI::process(AstroImage img, QPromise<void>& promise)
{
    /*float exponent = m_ui->skew->value();
    unsigned int nbDims = std::min(3U, img.getImg()->GetNumberOfComponentsPerPixel());

    using DuplicatorType = itk::ImageDuplicator<ImageType>;
    auto duplicator = DuplicatorType::New();
    duplicator->SetInputImage(img.getImg());
    duplicator->Update();

    img.setImg(duplicator->GetOutput());
    using IteratorType = itk::ImageRegionIterator<ImageType>;

    IteratorType it(img.getImg(), img.getImg()->GetRequestedRegion());
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
    }*/

    emit save(img);

    return img;
}
} // namespace astro
