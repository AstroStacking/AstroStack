#pragma once
#include <QtProcessing/multi2multiinterface.h>

namespace Ui
{
class StarRegister;
}
class QFileSystemModel;

namespace astro
{
/**
 * Registers images to the middle one
 */
class StarRegister
    : public QObject
    , public Multi2MultiInterface
{
    Q_OBJECT
public:
    ~StarRegister() override;

    QString name() const override;
    QString explanation() const override;

    Multi2MultiInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 QWidgets that will be displayed in the stack
 */
class StarRegisterGUI : public Multi2MultiInterfaceGUI
{
    Q_OBJECT
public:
    StarRegisterGUI(QWidget* parent);
    ~StarRegisterGUI() override;

    void setup(QJsonObject data) override;
    bool check() override;
    void process(const H5::H5File& group, const StartTask& startNewTask, const UpdateTask& updateTask,
                 QPromise<void>& promise) override;
    void restore(QSettings& settings) override;
    void save(QSettings& settings) override;

private slots:
    void setMinStarsValue(double val);
    void setApproximateMinStarsValue(int val);
    void setMaxStarsValue(double val);
    void setApproximateMaxStarsValue(int val);
    void setFullGraphValue(double val);
    void setApproximateFullGraphValue(int val);
    void setMaxRatioValue(double val);
    void setApproximateMaxRatioValue(int val);

private:
    void setupSlots();

    std::unique_ptr<Ui::StarRegister> m_ui;

    std::string m_inputsDatasetName;
    std::string m_intermediateGroupName;
    std::string m_greyDatasetName;
    std::string m_starsGroupName;
    std::string m_graphGroupName;
    std::string m_outputsDatasetName;
};

} // namespace astro
