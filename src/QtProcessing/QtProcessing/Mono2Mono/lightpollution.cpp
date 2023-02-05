#include "lightpollution.h"
#include "../ui_mono2monointerface.h"
#include "ui_lightpollution.h"
#include <Processing/lightpollution.h>

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
    img.setImg(processing::lightPollution(img.getImg()));

    emit save(img);

    return img;
}
} // namespace astro
