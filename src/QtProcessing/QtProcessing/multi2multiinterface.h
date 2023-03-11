#pragma once
#include <IO/hdf5.h>
#include <IO/io.h>
#include <QtProcessing/config.h>

#include <QtCore/QFileInfo>
#include <QtCore/QPromise>
#include <QtCore/QSettings>
#include <QtWidgets/QGroupBox>

namespace Ui
{
class Multi2MultiInterface;
}

namespace astro
{
/**
 * QWidgets that will be displayed in the stack
 */
class Multi2MultiInterfaceGUI : public QGroupBox
{
    Q_OBJECT
public:
    ASTRO_QTPROCESSING_EXPORT Multi2MultiInterfaceGUI(QWidget* parent);
    ASTRO_QTPROCESSING_EXPORT virtual ~Multi2MultiInterfaceGUI();

    using StartTask = std::function<void(int)>;
    using UpdateTask = std::function<void(int)>;

    /// Process the pipeline based on a promise.
    ASTRO_QTPROCESSING_EXPORT virtual void process(const H5::H5File& group, const StartTask& startNewTask,
                                                   const UpdateTask& updateTask, QPromise<void>& promise) = 0;
    /// Setting up inouts and outputs variables
    ASTRO_QTPROCESSING_EXPORT virtual void setup(QJsonObject data);

    ASTRO_QTPROCESSING_EXPORT virtual void restore(QSettings& settings) = 0;
    ASTRO_QTPROCESSING_EXPORT virtual void save(QSettings& settings) = 0;
    ASTRO_QTPROCESSING_EXPORT virtual bool check() = 0;
    ASTRO_QTPROCESSING_EXPORT bool isActive() const;
    ASTRO_QTPROCESSING_EXPORT QString getName() const;

protected:
    std::unique_ptr<Ui::Multi2MultiInterface> m_multiUi;
    QString m_name;
};

/**
 * Interface to process one image at a time
 */
class ASTRO_QTPROCESSING_EXPORT Multi2MultiInterface
{
public:
    Multi2MultiInterface();
    virtual ~Multi2MultiInterface();
    /// Name of the interface
    virtual QString name() const = 0;
    /// Return a string explaining what this interface does
    virtual QString explanation() const = 0;
    /// Generate a GUI to be added to the stack
    virtual Multi2MultiInterfaceGUI* generateGUI(QWidget* parent) const = 0;

    static const std::map<QString, Multi2MultiInterface*>& getPlugins();
};
} // namespace astro

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(astro::Multi2MultiInterface, "org.Astro.Processing.Multi2MultiInterface")
QT_END_NAMESPACE
