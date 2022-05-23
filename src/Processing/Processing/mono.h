#pragma once
#include <IO/io.h>

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
};

} // namespace astro

QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(astro::MonoInterface, "org.Astro.Processing.MonoInterface")
QT_END_NAMESPACE
