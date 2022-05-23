#pragma once
#include <IO/io.h>

#include <QtCore/QPromise>
#include <QtCore/QStringList>
#include <QtPlugin>
#include <QtWidgets/QWidget>

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
    class GUI : public QWidget
    {
    public:
        virtual ~GUI();

        /// Process a single image based on a promise.
        virtual ImageTypePtr process(ImageTypePtr img, QPromise<void>& promise) = 0;
    };
    /// Name of the interface
    virtual QString name() = 0;
    /// Return a string explaining what this interface does
    virtual QString explanation() = 0;
    /// Generate a GUI to be added to the stack
    virtual GUI* generateGUI(QWidget* parent) = 0;
};

} // namespace astro

QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(astro::MonoInterface, "org.Astro.Processing.MonoInterface")
QT_END_NAMESPACE