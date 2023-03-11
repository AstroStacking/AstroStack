#pragma once
#include <QtProcessing/multi2multiinterface.h>

namespace Ui
{
class Reader;
}
class QFileSystemModel;

namespace astro
{
/**
 * Reads images
 */
class Reader
    : public QObject
    , public Multi2MultiInterface
{
    Q_OBJECT
public:
    ~Reader() override;

    QString name() const override;
    QString explanation() const override;

    Multi2MultiInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 QWidgets that will be displayed in the stack
 */
class ReaderGUI : public Multi2MultiInterfaceGUI
{
    Q_OBJECT
public:
    ReaderGUI(QWidget* parent);
    ~ReaderGUI() override;

    void setup(QJsonObject data) override;
    bool check() override;
    void process(const H5::H5File& group, const StartTask& startNewTask,
                 const UpdateTask& updateTask, QPromise<void>& promise) override;
    void restore(QSettings& settings) override;
    void save(QSettings& settings) override;

private:
    std::unique_ptr<Ui::Reader> m_ui;
    std::unique_ptr<QFileSystemModel> m_model;

    QString m_outputDatasetName;
};

} // namespace astro
