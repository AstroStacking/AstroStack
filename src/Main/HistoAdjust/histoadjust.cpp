#include "histoadjust.h"
#include "ui_histoadjust.h"

#include <imagedata.h>

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QFileSystemModel>
#include <QtWidgets/QMenu>

namespace astro
{
HistoAdjust::HistoAdjust(QWidget* parent)
    : QWidget(parent)
    , m_ui(std::make_unique<Ui::HistoAdjust>())
{
    m_ui->setupUi(this);
    restore();
}

HistoAdjust::~HistoAdjust()
{
    save();
}

void HistoAdjust::restore()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("HistoAdjust");
    settings.endGroup();
}

void HistoAdjust::save()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("HistoAdjust");
    settings.endGroup();
}
} // namespace astro
