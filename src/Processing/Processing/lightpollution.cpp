#include "lightpollution.h"
#include "ui_lightpollution.h"
#include "ui_monointerface.h"

#include <Algos/gradient_estimation.h>

#include <itkSubtractImageFilter.h>

#include <QtWidgets/QDoubleSpinBox>

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
    ImageTypePtr light = estimateGradient(img.getImg());

    using SubtractImageFilterType = itk::SubtractImageFilter<ImageType, ImageType>;

    auto subtractFilter = SubtractImageFilterType::New();
    subtractFilter->SetInput1(img.getImg());
    subtractFilter->SetInput2(light);
    subtractFilter->Update();

    img.setImg(subtractFilter->GetOutput());

    emit save(img);

    return img;
}
} // namespace astro
