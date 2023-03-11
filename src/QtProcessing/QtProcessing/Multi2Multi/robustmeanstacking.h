#pragma once
#include <QtProcessing/multi2multiinterface.h>

namespace Ui
{
class RobustMeanStacking;
}

namespace astro
{
/**
 * Stacks the images, taking ribust mean of each pixel
 */
class RobustMeanStacking
    : public QObject
    , public Multi2MultiInterface
{
    Q_OBJECT
public:
    ~RobustMeanStacking() override;

    QString name() const override;

    QString explanation() const override;

    Multi2MultiInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 QWidgets that will be displayed in the stack
 */
class RobustMeanStackingGUI : public Multi2MultiInterfaceGUI
{
    Q_OBJECT
public:
    RobustMeanStackingGUI(QWidget* parent);
    ~RobustMeanStackingGUI() override;

    void setup(QJsonObject data) override;
    bool check() override;
    void process(const H5::H5File& group, const StartTask& startNewTask, const UpdateTask& updateTask,
                 QPromise<void>& promise) override;
    void restore(QSettings& settings) override;
    void save(QSettings& settings) override;

private slots:
    void outputFileBoxOpen();
    void setVarianceValue(double val);
    void setApproximateVarianceValue(int val);

private:
    void setupSlots();
    void setNextFilename(QFileInfo info, QString basename, long inc);
    void saveImg(const AstroImage& img);

    std::unique_ptr<Ui::RobustMeanStacking> m_ui;
    bool m_overwriteIfExists{true};
    std::string m_inputsDatasetName;
    std::string m_outputDatasetName;
};

} // namespace astro
