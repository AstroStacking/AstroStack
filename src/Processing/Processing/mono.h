#pragma once
#include <IO/io.h>

#include <QtCore/QPromise>
#include <QtWidgets/QFrame>

namespace astro
{
/**
  Interface to process one image at a time
 */
class MonoInterface
{
public:
    virtual ~MonoInterface();

    /**
     QWidgets that will be displayed in the stack
     */
    class GUI : public QFrame
    {
    public:
        GUI(QWidget* parent);
        virtual ~GUI();

        /// Process a single image based on a promise.
        virtual ImageTypePtr process(ImageTypePtr img, QPromise<void>& promise) = 0;
    };
    /// Name of the interface
    virtual QString name() const = 0;
    /// Return a string explaining what this interface does
    virtual QString explanation() const = 0;
    /// Generate a GUI to be added to the stack
    virtual GUI* generateGUI(QWidget* parent) const = 0;

    static const std::vector<MonoInterface*>& getPlugins();
};

} // namespace astro

QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(astro::MonoInterface, "org.Astro.Processing.MonoInterface")
QT_END_NAMESPACE
