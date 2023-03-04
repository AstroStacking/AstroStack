#pragma once
#include <IO/hdf5.h>
#include <IO/io.h>
#include <QtProcessing/config.h>

#include <QtCore/QFileInfo>
#include <QtCore/QPromise>
#include <QtWidgets/QGroupBox>

namespace Ui
{
class Multi2MonoInterface;
}

namespace astro
{
/**
 QWidgets that will be displayed in the stack
 */
class Multi2MonoInterfaceGUI : public QGroupBox
{
    Q_OBJECT
public:
    ASTRO_QTPROCESSING_EXPORT Multi2MonoInterfaceGUI(QWidget* parent);
    ASTRO_QTPROCESSING_EXPORT virtual ~Multi2MonoInterfaceGUI();

    /// Process the pipeline based on a promise.
    virtual AstroImage process(H5::Group group, QPromise<void>& promise) = 0;
    /// Setting up inouts and outputs variables
    ASTRO_QTPROCESSING_EXPORT virtual void setup(QJsonObject data);

    ASTRO_QTPROCESSING_EXPORT bool check();
    ASTRO_QTPROCESSING_EXPORT bool isActive();

protected:
    void setNextFilename(QFileInfo info, QString basename, long inc);

    void setupSlots();

signals:
    void save(const AstroImage& img);

private slots:
    void outputStateChanged(int state);
    void outputFileBoxOpen();
    void saveImg(const AstroImage& img);

protected:
    std::unique_ptr<Ui::Multi2MonoInterface> m_multiUi;
    QString m_name;

private:
    bool m_overwriteIfExists{true};
};

/**
 * Interface to process one image at a time
 */
class ASTRO_QTPROCESSING_EXPORT Multi2MonoInterface
{
public:
    Multi2MonoInterface();
    virtual ~Multi2MonoInterface();
    /// Name of the interface
    virtual QString name() const = 0;
    /// Return a string explaining what this interface does
    virtual QString explanation() const = 0;
    /// Generate a GUI to be added to the stack
    virtual Multi2MonoInterfaceGUI* generateGUI(QWidget* parent) const = 0;

    static const std::map<QString, Multi2MonoInterface*>& getPlugins();
};
} // namespace astro

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(astro::Multi2MonoInterface, "org.Astro.Processing.Multi2MonoInterface")
QT_END_NAMESPACE
