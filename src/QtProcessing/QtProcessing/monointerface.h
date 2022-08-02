#pragma once
#include <QtProcessing/config.h>

#include <IO/io.h>

#include <QtCore/QFileInfo>
#include <QtCore/QPromise>
#include <QtWidgets/QGroupBox>

namespace Ui
{
class MonoInterface;
}

namespace astro
{
/**
 QWidgets that will be displayed in the stack
 */
class MonoInterfaceGUI : public QGroupBox
{
    Q_OBJECT
public:
    ASTRO_QTPROCESSING_EXPORT MonoInterfaceGUI(QWidget* parent);
    ASTRO_QTPROCESSING_EXPORT virtual ~MonoInterfaceGUI();

    /// Process a single image based on a promise.
    virtual AstroImage process(AstroImage img, QPromise<void>& promise) = 0;
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
    std::unique_ptr<Ui::MonoInterface> m_monoUi;
    QString m_name;

private:
    bool m_overwriteIfExists{true};
};

/**
  Interface to process one image at a time
 */
class ASTRO_QTPROCESSING_EXPORT MonoInterface
{
public:
    MonoInterface();
    virtual ~MonoInterface();
    /// Name of the interface
    virtual QString name() const = 0;
    /// Return a string explaining what this interface does
    virtual QString explanation() const = 0;
    /// Generate a GUI to be added to the stack
    virtual MonoInterfaceGUI* generateGUI(QWidget* parent) const = 0;

    static const std::map<QString, MonoInterface*>& getPlugins();
};
} // namespace astro

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(astro::MonoInterface, "org.Astro.Processing.MonoInterface")
QT_END_NAMESPACE
