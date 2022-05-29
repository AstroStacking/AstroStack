#pragma once
#include <IO/io.h>

#include <QtCore/QPromise>
#include <QtWidgets/QGroupBox>

namespace Ui
{
class Mono;
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
    MonoInterfaceGUI(QWidget* parent);
    virtual ~MonoInterfaceGUI();

    /// Process a single image based on a promise.
    virtual ImageTypePtr process(ImageTypePtr img, QPromise<void>& promise) = 0;
    bool check();
    bool isActive();

protected:
    void setupSlots();

signals:
    void save(ImageTypePtr img);

private slots:
    void outputStateChanged(int state);
    void outputFileBoxOpen();
    void saveImg(ImageTypePtr img);

protected:
    std::unique_ptr<Ui::Mono> m_monoUi;

private:
    bool m_overwriteIfExists{true};
};

/**
  Interface to process one image at a time
 */
class MonoInterface
{
public:
    virtual ~MonoInterface();
    /// Name of the interface
    virtual QString name() const = 0;
    /// Return a string explaining what this interface does
    virtual QString explanation() const = 0;
    /// Generate a GUI to be added to the stack
    virtual MonoInterfaceGUI* generateGUI(QWidget* parent) const = 0;

    static const std::vector<MonoInterface*>& getPlugins();
};
} // namespace astro

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(astro::MonoInterface, "org.Astro.Processing.MonoInterface")
QT_END_NAMESPACE
