#include "lightpollution.h"
#include "ui_lightpollution.h"
#include "ui_monointerface.h"

#include <Algos/gradient_estimation.h>

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
    unsigned int nbDims = std::min(3U, img.getImg()->GetNumberOfComponentsPerPixel());
    ImageTypePtr light;

    switch (nbDims)
    {
        case 1:
            light = estimateGradient<1>(img.getImg());
            break;
        case 3:
            light = estimateGradient<3>(img.getImg());
            break;
        default:
            throw std::range_error("Not a supported number of channels");
    }

    emit save(img);

    return img;
}
} // namespace astro
