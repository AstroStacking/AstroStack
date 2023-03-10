#include "starregister.h"
#include "../ui_multi2multiinterface.h"
#include "ui_starregister.h"

#include <QtCore/QJsonObject>

namespace astro
{

StarRegister::~StarRegister() = default;

QString StarRegister::name() const
{
    return "StarRegister";
}

QString StarRegister::explanation() const
{
    return tr("Registers images on a middle one");
}

Multi2MultiInterfaceGUI* StarRegister::generateGUI(QWidget* parent) const
{
    return new StarRegisterGUI(parent);
}

StarRegisterGUI::StarRegisterGUI(QWidget* parent)
    : Multi2MultiInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::StarRegister>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_multiUi->setupUi(this, child);
    setTitle(tr("StarRegister"));

    setupSlots();
}

StarRegisterGUI::~StarRegisterGUI() = default;

void StarRegisterGUI::setupSlots()
{
    connect(m_ui->minStars, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setMinStarsValue);
    connect(m_ui->minStarsSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateMinStarsValue);
    connect(m_ui->maxStars, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setMaxStarsValue);
    connect(m_ui->maxStarsSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateMaxStarsValue);
    connect(m_ui->fullGraph, &QDoubleSpinBox::valueChanged, this, &StarRegisterGUI::setFullGraphValue);
    connect(m_ui->fullGraphSlider, &QSlider::valueChanged, this, &StarRegisterGUI::setApproximateFullGraphValue);
}

void StarRegisterGUI::setMinStarsValue(double val)
{
    m_ui->minStarsSlider->setValue(static_cast<int>(val));
}

void StarRegisterGUI::setApproximateMinStarsValue(int val)
{
    m_ui->minStars->setValue(val);
}

void StarRegisterGUI::setMaxStarsValue(double val)
{
    m_ui->maxStarsSlider->setValue(static_cast<int>(val));
}

void StarRegisterGUI::setApproximateMaxStarsValue(int val)
{
    m_ui->maxStars->setValue(val);
}

void StarRegisterGUI::setFullGraphValue(double val)
{
    m_ui->fullGraphSlider->setValue(static_cast<int>(val));
}

void StarRegisterGUI::setApproximateFullGraphValue(int val)
{
    m_ui->fullGraph->setValue(val);
}

void StarRegisterGUI::process(const H5::H5File& group, const std::function<void(int)>& startNewTask,
                        const std::function<void(int)>& updateTask, QPromise<void>& promise)
try
{
}
catch (const std::exception& e)
{
    promise.setException(std::current_exception());
}

void StarRegisterGUI::restore(QSettings& settings)
{
}

void StarRegisterGUI::save(QSettings& settings)
{
}

bool StarRegisterGUI::check()
{
    return true;
}

void StarRegisterGUI::setup(QJsonObject data)
{
    Multi2MultiInterfaceGUI::setup(data);
    auto outputs = data["Outputs"].toObject();
    m_outputDatasetName = outputs["data"].toObject()["dataset"].toString();
}

} // namespace astro
