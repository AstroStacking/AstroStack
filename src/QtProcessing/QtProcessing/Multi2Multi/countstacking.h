#pragma once
#include <QtProcessing/multi2multiinterface.h>

namespace Ui
{
class CountStacking;
}

namespace astro
{
/**
 * Stacks the images, taking count of each pixel
 */
class CountStacking
    : public QObject
    , public Multi2MultiInterface
{
    Q_OBJECT
public:
    ~CountStacking() override;

    QString name() const override;

    QString explanation() const override;

    Multi2MultiInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 QWidgets that will be displayed in the stack
 */
class CountStackingGUI : public Multi2MultiInterfaceGUI
{
    Q_OBJECT
public:
    CountStackingGUI(QWidget* parent);
    ~CountStackingGUI() override;

    void setup(QJsonObject data) override;
    bool check() override;
    void process(const H5::H5File& group, const StartTask& startNewTask, const UpdateTask& updateTask,
                 QPromise<void>& promise) override;
    void restore(QSettings& settings) override;
    void save(QSettings& settings) override;

private slots:
    void outputFileBoxOpen();

private:
    void setupSlots();
    void setNextFilename(QFileInfo info, QString basename, long inc);
    void saveImg(const AstroImage& img);

    std::unique_ptr<Ui::CountStacking> m_ui;
    bool m_overwriteIfExists{true};
    std::string m_inputsDatasetName;
    std::string m_outputDatasetName;
};

} // namespace astro
