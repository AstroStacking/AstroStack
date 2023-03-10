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
    void process(const H5::H5File& group, const std::function<void(int)>& startNewTask,
                 const std::function<void(int)>& updateTask, QPromise<void>& promise) override;
    void restore(QSettings& settings) override;
    void save(QSettings& settings) override;
    
private slots:
    void setMinStarsValue(double val);
    void setApproximateMinStarsValue(int val);
    void setMaxStarsValue(double val);
    void setApproximateMaxStarsValue(int val);
    void setFullGraphValue(double val);
    void setApproximateFullGraphValue(int val);

private:
    void setupSlots();

    std::unique_ptr<Ui::StarRegister> m_ui;

    std::string m_inputsDatasetName;
    QString m_intermediateGroupName;
    QString m_greyDatasetName;
    QString m_starsGroupName;
    QString m_graphGroupName;
    QString m_outputsDatasetName;
};

} // namespace astro
