#pragma once
#include <io.h>

#include <QtCore/QStringList>
#include <QtWidgets/QWidget>
#include <QtPlugin>

namespace astro
{

class InputInterface
{
public:
    virtual ~InputInterface();

    virtual QStringList filters() const = 0;
    virtual ImageTypePtr open(QString filename, QWidget* parent) const = 0;
};
}

QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(astro::InputInterface, "org.Astro.IO.InputInterface")
QT_END_NAMESPACE

