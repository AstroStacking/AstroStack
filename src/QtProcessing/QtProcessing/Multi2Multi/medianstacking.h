#pragma once
#include <QtProcessing/multi2multiinterface.h>

namespace Ui
{
class MedianStacking;
}

namespace astro
{
/**
 * Stacks the images, taking median of each pixel
 */
class MedianStacking
    : public QObject
    , public Multi2MultiInterface
{
    Q_OBJECT
public:
    ~MedianStacking() override;

    QString name() const override;

    QString explanation() const override;

    Multi2MultiInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 QWidgets that will be displayed in the stack
 */
class MedianStackingGUI : public Multi2MultiInterfaceGUI
{
    Q_OBJECT
public:
    MedianStackingGUI(QWidget* parent);
    ~MedianStackingGUI() override;

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

    std::unique_ptr<Ui::MedianStacking> m_ui;
    bool m_overwriteIfExists{true};
    std::string m_inputsDatasetName;
    std::string m_outputDatasetName;
};

} // namespace astro
